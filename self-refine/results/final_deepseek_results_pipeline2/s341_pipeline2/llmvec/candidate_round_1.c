#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        for (int i = 0; i < LEN_1D; i++) {
            float temp_b = b[i];
            int mask = temp_b > (float)0.;
            if (mask) {
                a[write_idx] = temp_b;
                write_idx += 1;
            }
        }
    }
}

        typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(4)));

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b) {
    const int VEC_SIZE = 8;

    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        int i = 0;

        // Vectorized main loop
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            v8f32 b_vec = *(v8f32*)&b[i];
            v8f32 mask_vec = b_vec > (v8f32){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

            // Extract mask bits
            int mask = __builtin_convertvector(mask_vec, v8f32) != (v8f32){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

            // Process each element based on mask
            for (int j = 0; j < VEC_SIZE; j++) {
                if (mask & (1 << j)) {
                    a[write_idx++] = b[i + j];
                }
            }
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float temp_b = b[i];
            if (temp_b > 0.0f) {
                a[write_idx] = temp_b;
                write_idx += 1;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s341(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s341(iterations, LEN_1D, a_vector, b_vector);
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
