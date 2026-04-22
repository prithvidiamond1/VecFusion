#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s342(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute mask and store original a values
        int mask[LEN_1D];
        float orig_a[LEN_1D];

        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (a[i] > (float)0.);
            orig_a[i] = a[i];
        }

        // Second loop: compute prefix sum (j values) using mask
        int j = -1;
        int j_vals[LEN_1D];

        for (int i = 0; i < LEN_1D; i++) {
            j_vals[i] = mask[i] ? (++j) : -1;
        }

        // Third loop: assign b values using computed indices
        for (int i = 0; i < LEN_1D; i++) {
            int idx = j_vals[i];
            a[i] = (idx != -1) ? b[idx] : orig_a[i];
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s342(int iterations, int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Allocate temporary arrays on stack if small, otherwise use VLAs
        int mask[LEN_1D];
        float orig_a[LEN_1D];
        int j_vals[LEN_1D];

        // First loop: compute mask and store original a values
        // Unroll by 4 for vectorization
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            float a0 = a[i+0];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];
            orig_a[i+0] = a0;
            orig_a[i+1] = a1;
            orig_a[i+2] = a2;
            orig_a[i+3] = a3;
            mask[i+0] = (a0 > 0.0f) ? 1 : 0;
            mask[i+1] = (a1 > 0.0f) ? 1 : 0;
            mask[i+2] = (a2 > 0.0f) ? 1 : 0;
            mask[i+3] = (a3 > 0.0f) ? 1 : 0;
        }
        for (; i < LEN_1D; i++) {
            orig_a[i] = a[i];
            mask[i] = (a[i] > 0.0f) ? 1 : 0;
        }

        // Second loop: compute prefix sum (j values) using mask
        // This is inherently sequential due to the running counter j
        int j = -1;
        for (i = 0; i < LEN_1D; i++) {
            if (mask[i]) {
                j_vals[i] = ++j;
            } else {
                j_vals[i] = -1;
            }
        }

        // Third loop: assign b values using computed indices
        // Unroll by 4 for vectorization
        i = 0;
        limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            int idx0 = j_vals[i+0];
            int idx1 = j_vals[i+1];
            int idx2 = j_vals[i+2];
            int idx3 = j_vals[i+3];
            a[i+0] = (idx0 != -1) ? b[idx0] : orig_a[i+0];
            a[i+1] = (idx1 != -1) ? b[idx1] : orig_a[i+1];
            a[i+2] = (idx2 != -1) ? b[idx2] : orig_a[i+2];
            a[i+3] = (idx3 != -1) ? b[idx3] : orig_a[i+3];
        }
        for (; i < LEN_1D; i++) {
            int idx = j_vals[i];
            a[i] = (idx != -1) ? b[idx] : orig_a[i];
        }
    }
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s342(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s342(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
