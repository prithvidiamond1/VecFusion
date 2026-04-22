#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original b[i+1] values before any updates
        float b_plus1[LEN_1D];
        for (int i = 1; i < LEN_1D-1; i++) {
            b_plus1[i] = b[i+1];
        }

        // Compute b[i] using stored original b[i+1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b_plus1[i] - e[i] * d[i];
        }

        // Now compute a[i] using updated b[i-1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}

        #include <stddef.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        int n = LEN_1D - 2; // number of elements from index 1 to LEN_1D-2

        // Step 1: Compute b[i] = b[i+1] - e[i]*d[i] for i in [1, LEN_1D-2)
        // We need to read b[i+1] before writing b[i], so process in a vectorized pass
        // using a temporary buffer stored on the stack or via VLA.
        // We'll use a VLA for the temp buffer.
        float b_plus1[LEN_1D];

        // Vectorized store of b[i+1] into b_plus1[i]
        {
            int i = 1;
            // Process 8 at a time
            for (; i <= n - 8; i += 8) {
                b_plus1[i+0] = b[i+1];
                b_plus1[i+1] = b[i+2];
                b_plus1[i+2] = b[i+3];
                b_plus1[i+3] = b[i+4];
                b_plus1[i+4] = b[i+5];
                b_plus1[i+5] = b[i+6];
                b_plus1[i+6] = b[i+7];
                b_plus1[i+7] = b[i+8];
            }
            for (; i < LEN_1D - 1; i++) {
                b_plus1[i] = b[i+1];
            }
        }

        // Vectorized compute b[i] = b_plus1[i] - e[i]*d[i]
        {
            int i = 1;
            for (; i <= n - 8; i += 8) {
                b[i+0] = b_plus1[i+0] - e[i+0] * d[i+0];
                b[i+1] = b_plus1[i+1] - e[i+1] * d[i+1];
                b[i+2] = b_plus1[i+2] - e[i+2] * d[i+2];
                b[i+3] = b_plus1[i+3] - e[i+3] * d[i+3];
                b[i+4] = b_plus1[i+4] - e[i+4] * d[i+4];
                b[i+5] = b_plus1[i+5] - e[i+5] * d[i+5];
                b[i+6] = b_plus1[i+6] - e[i+6] * d[i+6];
                b[i+7] = b_plus1[i+7] - e[i+7] * d[i+7];
            }
            for (; i < LEN_1D - 1; i++) {
                b[i] = b_plus1[i] - e[i] * d[i];
            }
        }

        // Vectorized compute a[i] = b[i-1] + c[i]*d[i]
        {
            int i = 1;
            for (; i <= n - 8; i += 8) {
                a[i+0] = b[i-1] + c[i+0] * d[i+0];
                a[i+1] = b[i+0] + c[i+1] * d[i+1];
                a[i+2] = b[i+1] + c[i+2] * d[i+2];
                a[i+3] = b[i+2] + c[i+3] * d[i+3];
                a[i+4] = b[i+3] + c[i+4] * d[i+4];
                a[i+5] = b[i+4] + c[i+5] * d[i+5];
                a[i+6] = b[i+5] + c[i+6] * d[i+6];
                a[i+7] = b[i+6] + c[i+7] * d[i+7];
            }
            for (; i < LEN_1D - 1; i++) {
                a[i] = b[i-1] + c[i] * d[i];
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
                s211(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s211(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
