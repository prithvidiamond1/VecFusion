#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        // Precompute all s values to break the recurrence chain
        float s_prev = 0.0f;

        // Vectorizable loop for s_next precomputation
        for (int i = 0; i < LEN_1D; i++) {
            float s_next = b[i] + c[i];
            a[i] = s_prev * e[i];
            b[i] = a[i] + d[i];
            s_prev = s_next;
        }
    }
}

        #include <stdint.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        float s_prev = 0.0f;

        // The loop has a carried dependency through s_prev (scalar), but the
        // body can be partially vectorized: a[i] = s_prev * e[i] depends on
        // s_prev from previous iteration (scalar chain), so we keep the scalar
        // structure but unroll 4x to help the compiler vectorize independent ops.

        int i = 0;
        // Scalar loop - the recurrence s_prev = b[i] + c[i] prevents full vectorization
        // but we unroll to expose ILP
        for (; i + 3 < LEN_1D; i += 4) {
            float s0 = b[i+0] + c[i+0];
            float s1 = b[i+1] + c[i+1];
            float s2 = b[i+2] + c[i+2];
            float s3 = b[i+3] + c[i+3];

            float a0 = s_prev * e[i+0];
            float a1 = s0    * e[i+1];
            float a2 = s1    * e[i+2];
            float a3 = s2    * e[i+3];

            a[i+0] = a0;
            a[i+1] = a1;
            a[i+2] = a2;
            a[i+3] = a3;

            b[i+0] = a0 + d[i+0];
            b[i+1] = a1 + d[i+1];
            b[i+2] = a2 + d[i+2];
            b[i+3] = a3 + d[i+3];

            s_prev = s3;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float s_next = b[i] + c[i];
            a[i] = s_prev * e[i];
            b[i] = a[i] + d[i];
            s_prev = s_next;
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
                s2251(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s2251(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
