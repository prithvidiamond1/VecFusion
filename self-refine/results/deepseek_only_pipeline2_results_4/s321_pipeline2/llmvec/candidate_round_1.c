#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s321(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        // Store original values to break dependency
        float* temp = (float*)malloc(LEN_1D * sizeof(float));
        memcpy(temp, a, LEN_1D * sizeof(float));

        float prev = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = temp[i] + prev * b[i];
            prev = a[i];
        }

        free(temp);
    }
}

        #include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef float v8f __attribute__((ext_vector_type(8)));
typedef int v8i __attribute__((ext_vector_type(8)));

void vectorized_s321(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        float* temp = (float*)malloc(LEN_1D * sizeof(float));
        memcpy(temp, a, LEN_1D * sizeof(float));

        float prev = a[0];
        int i = 1;

        // Vectorized main loop
        for (; i + 8 <= LEN_1D; i += 8) {
            // Load 8 elements from temp and b
            v8f temp_vec = *(v8f*)&temp[i];
            v8f b_vec = *(v8f*)&b[i];

            // Create prev vector: [prev, a[i], a[i+1], ..., a[i+6]]
            v8f prev_vec;
            prev_vec[0] = prev;
            for (int k = 1; k < 8; k++) {
                // Compute a[i+k-1] using the recurrence
                float prev_k = (k == 1) ? prev : prev_vec[k-1];
                prev_vec[k] = temp[i + k - 1] + prev_k * b[i + k - 1];
            }

            // Compute current 8 a values
            v8f a_vec = temp_vec + prev_vec * b_vec;

            // Store results
            *(v8f*)&a[i] = a_vec;

            // Update prev for next iteration
            prev = a_vec[7];
        }

        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] = temp[i] + prev * b[i];
            prev = a[i];
        }

        free(temp);
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
                s321(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s321(iterations, LEN_1D, a_vector, b_vector);
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
