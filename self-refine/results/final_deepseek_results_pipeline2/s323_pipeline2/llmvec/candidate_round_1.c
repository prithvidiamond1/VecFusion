#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s323(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original b values for this iteration to break dependency
        float b_prev = b[0];

        // First compute all c*d and c*e products (independent computations)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = c[i] * d[i];  // Temporarily store c*d in a[i]
        }

        // Now compute the recurrence using stored products
        for (int i = 1; i < LEN_1D; i++) {
            float cd = a[i];  // Retrieve c[i]*d[i]
            float ce = c[i] * e[i];
            float a_new = b_prev + cd;
            b_prev = a_new + ce;  // This becomes b[i] for next iteration
            b[i] = b_prev;
            a[i] = a_new;
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s323(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int outer_iters = iterations / 2;

    for (int nl = 0; nl < outer_iters; nl++) {
        // Store original b[0] value for this iteration
        float b_prev = b[0];

        // First compute all c*d products (independent computations)
        int i = 1;
        int len_minus_1 = LEN_1D - 1;

        // Vectorized computation of c*d
        for (; i + 3 < LEN_1D; i += 4) {
            float4 c_vec = *(float4*)&c[i];
            float4 d_vec = *(float4*)&d[i];
            float4 cd_vec = c_vec * d_vec;
            *(float4*)&a[i] = cd_vec;
        }

        // Scalar tail for c*d computation
        for (; i < LEN_1D; i++) {
            a[i] = c[i] * d[i];
        }

        // Now compute the recurrence using stored products
        i = 1;

        // Main vectorized recurrence loop
        for (; i + 3 < LEN_1D; i += 4) {
            // Load c*d products (stored in a[i])
            float4 cd_vec = *(float4*)&a[i];

            // Load c and e values
            float4 c_vec = *(float4*)&c[i];
            float4 e_vec = *(float4*)&e[i];

            // Compute c*e products
            float4 ce_vec = c_vec * e_vec;

            // Process 4 elements sequentially to maintain recurrence
            float b0 = b_prev + cd_vec.x;
            float b1 = b0 + ce_vec.x;
            b[i] = b1;
            a[i] = b0;

            float b2 = b1 + cd_vec.y;
            float b3 = b2 + ce_vec.y;
            b[i + 1] = b3;
            a[i + 1] = b2;

            float b4 = b3 + cd_vec.z;
            float b5 = b4 + ce_vec.z;
            b[i + 2] = b5;
            a[i + 2] = b4;

            float b6 = b5 + cd_vec.w;
            float b7 = b6 + ce_vec.w;
            b[i + 3] = b7;
            a[i + 3] = b6;

            b_prev = b7;
        }

        // Scalar tail for recurrence computation
        for (; i < LEN_1D; i++) {
            float cd = a[i];
            float ce = c[i] * e[i];
            float a_new = b_prev + cd;
            b_prev = a_new + ce;
            b[i] = b_prev;
            a[i] = a_new;
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
