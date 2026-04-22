#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute values for both branches into temporaries
        for (int i = 0; i < LEN_1D-1; ++i) {
            float temp_a = c[i] + d[i] * e[i];
            float temp_b = a[i] + d[i] * d[i];

            // Store results based on condition
            if (c[i] < (float)0.) {
                b[i] = temp_b;
            } else {
                a[i] = temp_a;
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    if (LEN_1D <= 1) return;

    int n = LEN_1D - 1;

    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;

        // Vectorized main loop
        for (; i + 3 < n; i += 4) {
            // Load vector data
            float c0 = c[i];
            float c1 = c[i+1];
            float c2 = c[i+2];
            float c3 = c[i+3];

            float d0 = d[i];
            float d1 = d[i+1];
            float d2 = d[i+2];
            float d3 = d[i+3];

            float e0 = e[i];
            float e1 = e[i+1];
            float e2 = e[i+2];
            float e3 = e[i+3];

            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];

            // Compute temporaries
            float temp_a0 = c0 + d0 * e0;
            float temp_a1 = c1 + d1 * e1;
            float temp_a2 = c2 + d2 * e2;
            float temp_a3 = c3 + d3 * e3;

            float temp_b0 = a0 + d0 * d0;
            float temp_b1 = a1 + d1 * d1;
            float temp_b2 = a2 + d2 * d2;
            float temp_b3 = a3 + d3 * d3;

            // Apply conditional stores
            if (c0 < 0.0f) b[i] = temp_b0; else a[i] = temp_a0;
            if (c1 < 0.0f) b[i+1] = temp_b1; else a[i+1] = temp_a1;
            if (c2 < 0.0f) b[i+2] = temp_b2; else a[i+2] = temp_a2;
            if (c3 < 0.0f) b[i+3] = temp_b3; else a[i+3] = temp_a3;
        }

        // Scalar cleanup tail
        for (; i < n; ++i) {
            float temp_a = c[i] + d[i] * e[i];
            float temp_b = a[i] + d[i] * d[i];

            if (c[i] < 0.0f) {
                b[i] = temp_b;
            } else {
                a[i] = temp_a;
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
                s1161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s1161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
