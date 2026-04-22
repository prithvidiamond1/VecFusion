#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s161(int iterations, int LEN_1D, float* a, float* b,float*c, float*d,float*e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] < (float)0.) {
                goto L20;
            }
            a[i] = c[i] + d[i] * e[i];
            goto L10;
L20:
            c[i+1] = a[i] + d[i] * d[i];
L10:
            ;
        }
    }
}

        #include <stdint.h>
#include <stdbool.h>

void vectorized_s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int total_iters = iterations / 2;
    for (int nl = 0; nl < total_iters; nl++) {
        int i = 0;
        // Process in chunks of 4 for potential vectorization
        for (; i <= LEN_1D - 5; i += 4) {
            // Load b values
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];

            // Check conditions and compute results
            bool cond0 = b0 < 0.0f;
            bool cond1 = b1 < 0.0f;
            bool cond2 = b2 < 0.0f;
            bool cond3 = b3 < 0.0f;

            // Compute both paths for each element
            float a_val0 = c[i] + d[i] * e[i];
            float a_val1 = c[i+1] + d[i+1] * e[i+1];
            float a_val2 = c[i+2] + d[i+2] * e[i+2];
            float a_val3 = c[i+3] + d[i+3] * e[i+3];

            float c_next0 = a[i] + d[i] * d[i];
            float c_next1 = a[i+1] + d[i+1] * d[i+1];
            float c_next2 = a[i+2] + d[i+2] * d[i+2];
            float c_next3 = a[i+3] + d[i+3] * d[i+3];

            // Select based on condition
            a[i] = cond0 ? a[i] : a_val0;
            a[i+1] = cond1 ? a[i+1] : a_val1;
            a[i+2] = cond2 ? a[i+2] : a_val2;
            a[i+3] = cond3 ? a[i+3] : a_val3;

            c[i+1] = cond0 ? c_next0 : c[i+1];
            c[i+2] = cond1 ? c_next1 : c[i+2];
            c[i+3] = cond2 ? c_next2 : c[i+3];
            c[i+4] = cond3 ? c_next3 : c[i+4];
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D - 1; ++i) {
            if (b[i] < 0.0f) {
                c[i+1] = a[i] + d[i] * d[i];
            } else {
                a[i] = c[i] + d[i] * e[i];
            }
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
                s161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
