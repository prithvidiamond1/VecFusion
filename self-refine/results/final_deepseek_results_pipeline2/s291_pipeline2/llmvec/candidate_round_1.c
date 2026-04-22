#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s291(int iterations,int LEN_1D, float* a,float* b)
{
    int im1;
    float prev_b;

    for (int nl = 0; nl < 2*iterations; nl++) {
        im1 = LEN_1D-1;
        prev_b = b[im1];

        for (int i = 0; i < LEN_1D; i++) {
            float current_b = b[i];
            a[i] = (current_b + prev_b) * (float).5;
            prev_b = current_b;
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b) {
    int total_outer_iterations = 2 * iterations;

    for (int nl = 0; nl < total_outer_iterations; nl++) {
        int im1 = LEN_1D - 1;
        float prev_b = b[im1];

        // Vectorized main loop
        int i = 0;
        int vec_limit = LEN_1D - (LEN_1D % 4);

        if (vec_limit > 0) {
            // Load first vector with proper alignment for the recurrence
            float4 current_vec = *(float4*)&b[0];
            float prev_scalar = prev_b;

            for (; i < vec_limit; i += 4) {
                // Create vector of previous values: [prev_scalar, current_vec[0], current_vec[1], current_vec[2]]
                float4 prev_vec = {prev_scalar, current_vec[0], current_vec[1], current_vec[2]};

                // Compute average
                float4 avg_vec = (current_vec + prev_vec) * 0.5f;

                // Store result
                *(float4*)&a[i] = avg_vec;

                // Update prev_scalar for next iteration
                prev_scalar = current_vec[3];

                // Load next vector if not at the end
                if (i + 4 < vec_limit) {
                    current_vec = *(float4*)&b[i + 4];
                }
            }

            // Update prev_b for scalar tail
            prev_b = prev_scalar;
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float current_b = b[i];
            a[i] = (current_b + prev_b) * 0.5f;
            prev_b = current_b;
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
                s291(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s291(iterations, LEN_1D, a_vector, b_vector);
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
