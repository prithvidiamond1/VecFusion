#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s316(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        float temp_x = x;
        for (int i = 1; i < LEN_1D; ++i) {
            float current = a[i];
            if (current < temp_x) {
                temp_x = current;
            }
        }
        x = temp_x;
    }
    return x;
}

        typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(32)));

float vectorized_s316(int iterations, int LEN_1D, float* a) {
    float x = 0.0f;
    int outer_iterations = iterations * 5;

    for (int nl = 0; nl < outer_iterations; nl++) {
        float min_val = a[0];

        // Vectorized reduction
        int i = 1;
        int vec_limit = ((LEN_1D - 1) / 8) * 8 + 1;

        if (vec_limit > 1) {
            v8f32 min_vec = (v8f32){min_val, min_val, min_val, min_val, 
                                   min_val, min_val, min_val, min_val};

            for (; i < vec_limit; i += 8) {
                v8f32 current_vec = *(v8f32*)&a[i];
                v8f32 mask = current_vec < min_vec;
                min_vec = (mask ? current_vec : min_vec);
            }

            // Horizontal reduction
            float temp_min = min_val;
            float* min_arr = (float*)&min_vec;
            for (int j = 0; j < 8; j++) {
                if (min_arr[j] < temp_min) {
                    temp_min = min_arr[j];
                }
            }
            min_val = temp_min;
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            float current = a[i];
            if (current < min_val) {
                min_val = current;
            }
        }

        x = min_val;
    }

    return x;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s316(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s316(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
