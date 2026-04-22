#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    float x;
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x-(float)1.0;
            b[i] = x;
        }
    }
}

        #include <stdint.h>
#include <stddef.h>

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c) {
    float x;
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        #if defined(__clang__) && defined(__SSE__)
        // Process 4 elements at a time using SSE
        for (; i <= LEN_1D - 4; i += 4) {
            // Load b[i..i+3] and c[i..i+3]
            __builtin_ia32_loadups(b + i, (__v4sf){0});
            __builtin_ia32_loadups(c + i, (__v4sf){0});
            // Compute b[i]*c[i] for 4 elements
            __v4sf bc = __builtin_ia32_mulps((__v4sf){b[i], b[i+1], b[i+2], b[i+3]}, 
                                            (__v4sf){c[i], c[i+1], c[i+2], c[i+3]});
            // Load a[LEN_1D-i-1 .. LEN_1D-i-4] (reverse order)
            __v4sf a_rev = (__v4sf){a[LEN_1D-i-1], a[LEN_1D-i-2], a[LEN_1D-i-3], a[LEN_1D-i-4]};
            // x = a_rev + bc
            __v4sf x_vec = __builtin_ia32_addps(a_rev, bc);
            // a[i..i+3] = x - 1.0f
            __v4sf a_new = __builtin_ia32_subps(x_vec, (__v4sf){1.0f, 1.0f, 1.0f, 1.0f});
            __builtin_ia32_storeups(a + i, a_new);
            // b[i..i+3] = x
            __builtin_ia32_storeups(b + i, x_vec);
        }
        #endif
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            x = a[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
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
