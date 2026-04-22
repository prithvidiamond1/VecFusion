#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s314(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float max_val = a[0];
        int i;

        // Process in chunks to allow potential vectorization
        for (i = 1; i < LEN_1D; i++) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }

        x = max_val;
    }
    return x;
}

        #include <stdint.h>
#include <float.h>

float vectorized_s314(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        float max_val = a[0];
        int i = 1;

        // Vector width: process 8 floats at a time
        #define VEC_WIDTH 8
        typedef float float8 __attribute__((vector_size(8 * sizeof(float))));

        // Initialize vector of max values
        float8 vmax = {max_val, max_val, max_val, max_val,
                       max_val, max_val, max_val, max_val};

        int limit = LEN_1D - VEC_WIDTH + 1;
        for (; i < limit; i += VEC_WIDTH) {
            float8 va;
            __builtin_memcpy(&va, &a[i], sizeof(float8));
            // Element-wise max using ternary via comparison
            float8 mask = va > vmax;
            // Use mask to select: mask is all-ones or all-zeros per element
            // Portable approach: use conditional assignment per lane
            vmax = (float8){
                (va[0] > vmax[0]) ? va[0] : vmax[0],
                (va[1] > vmax[1]) ? va[1] : vmax[1],
                (va[2] > vmax[2]) ? va[2] : vmax[2],
                (va[3] > vmax[3]) ? va[3] : vmax[3],
                (va[4] > vmax[4]) ? va[4] : vmax[4],
                (va[5] > vmax[5]) ? va[5] : vmax[5],
                (va[6] > vmax[6]) ? va[6] : vmax[6],
                (va[7] > vmax[7]) ? va[7] : vmax[7],
            };
        }

        // Reduce vector max to scalar
        float reduced = vmax[0];
        for (int k = 1; k < VEC_WIDTH; k++) {
            if (vmax[k] > reduced) reduced = vmax[k];
        }
        max_val = reduced;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }

        x = max_val;
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
