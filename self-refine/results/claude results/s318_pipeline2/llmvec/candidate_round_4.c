#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s318(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        index = 0;
        max = fabsf(a[0]);

        // First pass: find max value (vectorizable max-reduction)
        for (int i = 1; i < LEN_1D; i++) {
            float val = fabsf(a[i * inc]);
            if (val > max) max = val;
        }

        // Second pass: find first index where value equals max
        // Use a flag array approach to avoid loop-carried dependency on index
        int found = 0;
        for (int i = 1; i < LEN_1D; i++) {
            float val = fabsf(a[i * inc]);
            int is_max = (val == max);
            int update = is_max & (found == 0);
            index = update ? i : index;
            found = found | update;
        }

        // Fix index: if max is a[0], index should remain 0
        index = (fabsf(a[0]) == max) ? 0 : index;

        chksum = max + (float) index;
    }
    return max + index + 1;
}

        #include <math.h>
#include <stdint.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc)
{
    int index;
    float max, chksum;
    chksum = 0.0f;
    max = 0.0f;
    index = 0;

    for (int nl = 0; nl < iterations/2; nl++) {
        index = 0;
        max = fabsf(a[0]);

        /* First pass: find max value */
        int i = 1;
        for (; i <= LEN_1D - 4; i += 4) {
            float v0 = fabsf(a[i]);
            float v1 = fabsf(a[i+1]);
            float v2 = fabsf(a[i+2]);
            float v3 = fabsf(a[i+3]);
            if (v0 > max) max = v0;
            if (v1 > max) max = v1;
            if (v2 > max) max = v2;
            if (v3 > max) max = v3;
        }
        for (; i < LEN_1D; i++) {
            float val = fabsf(a[i]);
            if (val > max) max = val;
        }

        /* Second pass: find first index where value equals max */
        int found = 0;
        /* Check index 0 first */
        if (fabsf(a[0]) == max) {
            index = 0;
            found = 1;
        }

        i = 1;
        for (; i <= LEN_1D - 4 && !found; i += 4) {
            float v0 = fabsf(a[i]);
            float v1 = fabsf(a[i+1]);
            float v2 = fabsf(a[i+2]);
            float v3 = fabsf(a[i+3]);
            int m0 = (v0 == max);
            int m1 = (v1 == max);
            int m2 = (v2 == max);
            int m3 = (v3 == max);
            if (m0 & !found) { index = i;   found = 1; }
            if (m1 & !found) { index = i+1; found = 1; }
            if (m2 & !found) { index = i+2; found = 1; }
            if (m3 & !found) { index = i+3; found = 1; }
        }
        for (; i < LEN_1D && !found; i++) {
            float val = fabsf(a[i]);
            if (val == max) { index = i; found = 1; }
        }

        chksum = max + (float)index;
    }
    return max + index + 1;
}

        static uint32_t next_u32(uint32_t *state) {
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }

        static void fill_i32(int *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }
        }

        static void fill_f32(float *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
            }
        }

        static void fill_f64(double *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
            }
        }

        int main(void) {
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; int inc = 7; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s318(iterations, LEN_1D, a_scalar, inc); ret_vector = vectorized_s318(iterations, LEN_1D, a_vector, inc); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
