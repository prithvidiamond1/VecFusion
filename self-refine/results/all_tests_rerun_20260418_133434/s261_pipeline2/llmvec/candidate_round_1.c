#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s261(int iterations,int LEN_1D, float* a,float* b,float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original c[i-1] values before they're modified
        float c_prev = c[0];

        for (int i = 1; i < LEN_1D; ++i) {
            // Compute a[i] using original c[i-1] (stored in c_prev)
            float t1 = a[i] + b[i];
            a[i] = t1 + c_prev;

            // Update c[i] and save original for next iteration
            float original_c_i = c[i];
            c[i] = c[i] * d[i];

            // Save current original c[i] for next iteration's a[i+1]
            c_prev = original_c_i;
        }

        // Handle the last c[0] update separately
        c[0] = c[0] * d[0];
    }
}

        #include <string.h>

void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // a[i] = a[i] + b[i] + c[i-1] (original c[i-1])
        // c[i] = c[i] * d[i]
        // These use original c values, so we can vectorize both loops independently.

        // Vectorize a[i] update: a[i] += b[i] + c_orig[i-1]
        // We read c[i-1] before c is modified, so process in chunks
        // using a local buffer for the shifted c values.

        // Process a[i] = a[i] + b[i] + c[i-1] (original c)
        // We can do this in-place since c[i-1] is read before c[i] is written
        // and a[] doesn't alias c[].

        int i = 1;
        // Unroll by 8 for vectorization
        for (; i <= LEN_1D - 8; i += 8) {
            float c0 = c[i-1];
            float c1 = c[i+0];
            float c2 = c[i+1];
            float c3 = c[i+2];
            float c4 = c[i+3];
            float c5 = c[i+4];
            float c6 = c[i+5];
            float c7 = c[i+6];

            a[i+0] = a[i+0] + b[i+0] + c0;
            a[i+1] = a[i+1] + b[i+1] + c1;
            a[i+2] = a[i+2] + b[i+2] + c2;
            a[i+3] = a[i+3] + b[i+3] + c3;
            a[i+4] = a[i+4] + b[i+4] + c4;
            a[i+5] = a[i+5] + b[i+5] + c5;
            a[i+6] = a[i+6] + b[i+6] + c6;
            a[i+7] = a[i+7] + b[i+7] + c7;
        }
        // Scalar tail for a[]
        for (; i < LEN_1D; i++) {
            a[i] = a[i] + b[i] + c[i-1];
        }

        // Now update c[] independently: c[i] = c[i] * d[i] for i in [0, LEN_1D)
        i = 0;
        for (; i <= LEN_1D - 8; i += 8) {
            c[i+0] = c[i+0] * d[i+0];
            c[i+1] = c[i+1] * d[i+1];
            c[i+2] = c[i+2] * d[i+2];
            c[i+3] = c[i+3] * d[i+3];
            c[i+4] = c[i+4] * d[i+4];
            c[i+5] = c[i+5] * d[i+5];
            c[i+6] = c[i+6] * d[i+6];
            c[i+7] = c[i+7] * d[i+7];
        }
        // Scalar tail for c[]
        for (; i < LEN_1D; i++) {
            c[i] = c[i] * d[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s261(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s261(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
