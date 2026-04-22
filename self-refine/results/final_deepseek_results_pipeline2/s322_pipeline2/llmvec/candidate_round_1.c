#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s322(int iterations, int LEN_1D, float* a, float* b, float* c) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original values to break loop-carried dependency
        float prev2 = a[0];
        float prev1 = a[1];

        // Main computation loop with scalar replacement
        for (int i = 2; i < LEN_1D; i++) {
            float current = a[i];
            float temp = current + prev1 * b[i] + prev2 * c[i];
            prev2 = prev1;
            prev1 = temp;
            a[i] = temp;
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s322(int iterations, int LEN_1D, float* a, float* b, float* c) {
    int loop_count = iterations / 2;

    for (int nl = 0; nl < loop_count; nl++) {
        // Store original values for scalar replacement
        float prev2 = a[0];
        float prev1 = a[1];

        // Process elements 2..LEN_1D-1
        int i = 2;

        // Vectorized main loop (process 4 elements at a time)
        for (; i + 3 < LEN_1D; i += 4) {
            // Load current a values
            float4 current_vec = {a[i], a[i+1], a[i+2], a[i+3]};

            // Load b and c values
            float4 b_vec = {b[i], b[i+1], b[i+2], b[i+3]};
            float4 c_vec = {c[i], c[i+1], c[i+2], c[i+3]};

            // Compute results sequentially within the vector
            float results[4];

            // First element in vector
            results[0] = current_vec.x + prev1 * b_vec.x + prev2 * c_vec.x;
            prev2 = prev1;
            prev1 = results[0];

            // Second element
            results[1] = current_vec.y + prev1 * b_vec.y + prev2 * c_vec.y;
            prev2 = prev1;
            prev1 = results[1];

            // Third element
            results[2] = current_vec.z + prev1 * b_vec.z + prev2 * c_vec.z;
            prev2 = prev1;
            prev1 = results[2];

            // Fourth element
            results[3] = current_vec.w + prev1 * b_vec.w + prev2 * c_vec.w;
            prev2 = prev1;
            prev1 = results[3];

            // Store results back to a
            a[i] = results[0];
            a[i+1] = results[1];
            a[i+2] = results[2];
            a[i+3] = results[3];
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float current = a[i];
            float temp = current + prev1 * b[i] + prev2 * c[i];
            prev2 = prev1;
            prev1 = temp;
            a[i] = temp;
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
                s322(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s322(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
