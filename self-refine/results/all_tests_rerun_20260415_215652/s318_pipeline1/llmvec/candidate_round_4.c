#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s318(int iterations,int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;
        for (int i = 1; i < LEN_1D; i++) {
            if (fabsf(a[k]) <= max) {
                goto L5;
            }
            index = i;
            max = fabsf(a[k]);
L5:
            k += inc;
        }
        chksum = max + (float) index;
    }
    return max + index + 1;
}

        #include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc)
{
    int index = 0;
    float max = 0.0f;
    float chksum = 0.0f;

    for (int nl = 0; nl < iterations/2; nl++) {
        int k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        int chunk = (LEN_1D - 1) / 4;
        int rem_start = 1 + chunk * 4;

        for (int ci = 0; ci < chunk; ci++) {
            int base = 1 + ci * 4;

            float v0 = fabsf(a[k]); k += inc;
            float v1 = fabsf(a[k]); k += inc;
            float v2 = fabsf(a[k]); k += inc;
            float v3 = fabsf(a[k]); k += inc;

            if (v0 > max) { max = v0; index = base + 0; }
            if (v1 > max) { max = v1; index = base + 1; }
            if (v2 > max) { max = v2; index = base + 2; }
            if (v3 > max) { max = v3; index = base + 3; }
        }

        for (int i2 = rem_start; i2 < LEN_1D; i2++) {
            float v = fabsf(a[k]);
            if (v > max) {
                max = v;
                index = i2;
            }
            k += inc;
        }

        chksum = max + (float)index;
    }
    return chksum;
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
