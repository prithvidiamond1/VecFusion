#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First: compute all x values into a temporary array
        float x[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            x[i] = a[LEN_1D-i-1] + b[i] * c[i];
        }

        // Second: update a and b arrays using pre-computed x values
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = x[i] - (float)1.0;
            b[i] = x[i];
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    // Compute vectorized width
    const int VEC_WIDTH = 4;
    const int VEC_ITERS = LEN_1D / VEC_WIDTH;
    const int VEC_REMAIN = LEN_1D % VEC_WIDTH;

    for (int nl = 0; nl < iterations; nl++) {
        // Allocate temporary array for x values
        float x[LEN_1D];

        // Vectorized computation of x[i] = a[LEN_1D-i-1] + b[i] * c[i]
        for (int i = 0; i < VEC_ITERS; i++) {
            int base_idx = i * VEC_WIDTH;

            // Load b[i] and c[i] vectors
            float4 b_vec = *(float4*)&b[base_idx];
            float4 c_vec = *(float4*)&c[base_idx];

            // Compute reverse indices for a[LEN_1D-i-1]
            int4 rev_idx = (int4){LEN_1D - base_idx - 1, 
                                  LEN_1D - base_idx - 2,
                                  LEN_1D - base_idx - 3,
                                  LEN_1D - base_idx - 4};

            // Gather a values from reverse indices
            float4 a_rev_vec = (float4){a[rev_idx.x], a[rev_idx.y], 
                                        a[rev_idx.z], a[rev_idx.w]};

            // Compute x = a_rev + b * c
            float4 x_vec = a_rev_vec + b_vec * c_vec;

            // Store x vector
            *(float4*)&x[base_idx] = x_vec;
        }

        // Scalar tail for x computation
        for (int i = VEC_ITERS * VEC_WIDTH; i < LEN_1D; i++) {
            x[i] = a[LEN_1D - i - 1] + b[i] * c[i];
        }

        // Vectorized update of a[i] = x[i] - 1.0 and b[i] = x[i]
        for (int i = 0; i < VEC_ITERS; i++) {
            int base_idx = i * VEC_WIDTH;

            // Load x vector
            float4 x_vec = *(float4*)&x[base_idx];

            // Compute a = x - 1.0
            float4 a_vec = x_vec - (float4)1.0f;

            // Store results
            *(float4*)&a[base_idx] = a_vec;
            *(float4*)&b[base_idx] = x_vec;
        }

        // Scalar tail for updates
        for (int i = VEC_ITERS * VEC_WIDTH; i < LEN_1D; i++) {
            a[i] = x[i] - 1.0f;
            b[i] = x[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s281(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s281(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
