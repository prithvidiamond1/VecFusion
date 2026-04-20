#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s314(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            float current = a[i];
            if (current > x) {
                x = current;
            }
        }
    }
    return x;
}

        #include <stddef.h>

float vectorized_s314(int iterations, int LEN_1D, float* a) {
    float x;
    int outer_iterations = iterations * 5;

    for (int nl = 0; nl < outer_iterations; nl++) {
        x = a[0];

        // Vectorized reduction for max operation
        typedef float v4f __attribute__((vector_size(16)));
        const int VEC_SIZE = 4;

        // Process vectorizable portion
        int i = 0;
        v4f vmax = {x, x, x, x};

        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            v4f vcurrent = *(v4f*)&a[i];
            vmax = vcurrent > vmax ? vcurrent : vmax;
        }

        // Extract horizontal maximum from vector
        float max_arr[4];
        *(v4f*)max_arr = vmax;
        float vec_max = max_arr[0];
        for (int j = 1; j < 4; j++) {
            if (max_arr[j] > vec_max) {
                vec_max = max_arr[j];
            }
        }

        // Combine vector result with scalar initialization
        if (vec_max > x) {
            x = vec_max;
        }

        // Process remaining elements
        for (; i < LEN_1D; i++) {
            float current = a[i];
            if (current > x) {
                x = current;
            }
        }
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
                ret_scalar = s314(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s314(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
