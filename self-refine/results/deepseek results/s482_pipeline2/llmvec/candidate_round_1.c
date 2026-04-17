#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    // Hoist invariant break detection outside nl loop
    int break_index = LEN_1D;
    for (int i = 0; i < LEN_1D; i++) {
        if (c[i] > b[i]) {
            break_index = i + 1; // Include current iteration
            break;
        }
    }

    // Main arithmetic loop with vectorizable structure
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < break_index; i++) {
            a[i] += b[i] * c[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    // Hoist invariant break detection outside nl loop
    int break_index = LEN_1D;
    for (int i = 0; i < LEN_1D; i++) {
        if (c[i] > b[i]) {
            break_index = i + 1;
            break;
        }
    }

    // Main arithmetic loop with vectorized structure
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        // Process 8 elements at a time using compiler vector types
        typedef float float4 __attribute__((vector_size(16)));
        typedef float float8 __attribute__((vector_size(32)));

        for (; i + 8 <= break_index; i += 8) {
            float8 va, vb, vc;
            __builtin_memcpy(&va, a + i, 32);
            __builtin_memcpy(&vb, b + i, 32);
            __builtin_memcpy(&vc, c + i, 32);
            va = va + vb * vc;
            __builtin_memcpy(a + i, &va, 32);
        }

        // Process 4 elements at a time
        for (; i + 4 <= break_index; i += 4) {
            float4 va, vb, vc;
            __builtin_memcpy(&va, a + i, 16);
            __builtin_memcpy(&vb, b + i, 16);
            __builtin_memcpy(&vc, c + i, 16);
            va = va + vb * vc;
            __builtin_memcpy(a + i, &va, 16);
        }

        // Scalar cleanup tail
        for (; i < break_index; i++) {
            a[i] += b[i] * c[i];
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
                s482(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s482(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
