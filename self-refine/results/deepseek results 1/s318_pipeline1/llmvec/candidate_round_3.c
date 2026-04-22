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
    int k, index;
    float max, chksum;
    chksum = 0.0f;
    max = 0.0f;
    index = 0;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        float cur_max = max;
        int cur_idx = 0;

        int i = 1;
        int end4 = LEN_1D - 3;

        for (; i < end4; i += 4) {
            float v0 = fabsf(a[k]);
            float v1 = fabsf(a[k + inc]);
            float v2 = fabsf(a[k + 2*inc]);
            float v3 = fabsf(a[k + 3*inc]);

            if (v0 > cur_max) { cur_max = v0; cur_idx = i; }
            if (v1 > cur_max) { cur_max = v1; cur_idx = i+1; }
            if (v2 > cur_max) { cur_max = v2; cur_idx = i+2; }
            if (v3 > cur_max) { cur_max = v3; cur_idx = i+3; }

            k += 4*inc;
        }

        for (; i < LEN_1D; i++) {
            float v = fabsf(a[k]);
            if (v > cur_max) {
                cur_max = v;
                cur_idx = i;
            }
            k += inc;
        }

        max = cur_max;
        index = cur_idx;

        chksum = max + (float)index;
    }
    return max + (float)index + 1;
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; int inc = 7; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s318(iterations, LEN_1D, a_scalar, inc); ret_vector = vectorized_s318(iterations, LEN_1D, a_vector, inc); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
