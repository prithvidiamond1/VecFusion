#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s323(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Compute prefix sum for b first
        float b_prev = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b_prev + c[i] * (d[i] + e[i]);
            b_prev = b[i];
        }

        // Now compute a in a separate vectorizable loop
        a[1] = b[0] + c[1] * d[1];
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}

        #include <stddef.h>

void vectorized_s323(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int loop_count = iterations / 2;

    for (int nl = 0; nl < loop_count; nl++) {
        // Prefix sum for b - scalar sequential (not vectorizable due to recurrence)
        float b_prev = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b_prev + c[i] * (d[i] + e[i]);
            b_prev = b[i];
        }

        // Compute a[1] separately
        a[1] = b[0] + c[1] * d[1];

        // Vectorized computation for a[i] = b[i-1] + c[i] * d[i]
        int i = 2;
        for (; i + 3 < LEN_1D; i += 4) {
            // Load b[i-1] values for indices i, i+1, i+2, i+3
            float b_prev_0 = b[i-1];
            float b_prev_1 = b[i];
            float b_prev_2 = b[i+1];
            float b_prev_3 = b[i+2];

            // Compute c[i] * d[i] for four consecutive elements
            float prod_0 = c[i] * d[i];
            float prod_1 = c[i+1] * d[i+1];
            float prod_2 = c[i+2] * d[i+2];
            float prod_3 = c[i+3] * d[i+3];

            // Store results
            a[i] = b_prev_0 + prod_0;
            a[i+1] = b_prev_1 + prod_1;
            a[i+2] = b_prev_2 + prod_2;
            a[i+3] = b_prev_3 + prod_3;
        }

        // Scalar tail for a computation
        for (; i < LEN_1D; i++) {
            a[i] = b[i-1] + c[i] * d[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s323(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s323(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
