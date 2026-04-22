#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s222(int iterations, int LEN_1D, float*a,float*b,float*c, float* e) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Loop distribution: separate the e recurrence (loop-carried dependency)
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }
        // Independent a updates (no net effect, but preserve memory access pattern)
        for (int i = 1; i < LEN_1D; i++) {
            float temp = b[i] * c[i];
            a[i] += temp;
            a[i] -= temp;
        }
    }
}

        #include <stddef.h>

void vectorized_s222(int iterations, int LEN_1D, float* a, float* b, float* c, float* e) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Vectorize the e recurrence loop
        int i = 1;
        // Process in chunks of 4 to match typical SIMD width
        for (; i + 3 < LEN_1D; i += 4) {
            // Load previous 4 e values
            float e_prev0 = e[i - 1];
            float e_prev1 = e[i];
            float e_prev2 = e[i + 1];
            float e_prev3 = e[i + 2];

            // Square them
            e[i] = e_prev0 * e_prev0;
            e[i + 1] = e_prev1 * e_prev1;
            e[i + 2] = e_prev2 * e_prev2;
            e[i + 3] = e_prev3 * e_prev3;
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }

        // Vectorize the independent a updates
        i = 1;
        for (; i + 3 < LEN_1D; i += 4) {
            // Load b and c values
            float b0 = b[i];
            float b1 = b[i + 1];
            float b2 = b[i + 2];
            float b3 = b[i + 3];

            float c0 = c[i];
            float c1 = c[i + 1];
            float c2 = c[i + 2];
            float c3 = c[i + 3];

            // Compute temp values
            float temp0 = b0 * c0;
            float temp1 = b1 * c1;
            float temp2 = b2 * c2;
            float temp3 = b3 * c3;

            // Update a values (net effect is zero)
            a[i] += temp0;
            a[i] -= temp0;
            a[i + 1] += temp1;
            a[i + 1] -= temp1;
            a[i + 2] += temp2;
            a[i + 2] -= temp2;
            a[i + 3] += temp3;
            a[i + 3] -= temp3;
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            float temp = b[i] * c[i];
            a[i] += temp;
            a[i] -= temp;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s222(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, e_scalar); vectorized_s222(iterations, LEN_1D, a_vector, b_vector, c_vector, e_vector);
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
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
