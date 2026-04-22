#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s481(int iterations,int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < (float)0.) {
                return;
            }
            a[i] += b[i] * c[i];
        }
    }
}

        #include <stddef.h>

typedef float float32x4_t __attribute__((ext_vector_type(4)));
typedef int int32x4_t __attribute__((ext_vector_type(4)));

void vectorized_s481(int iterations, int LEN_1D, float* restrict a, float* restrict b, float* restrict c, float* restrict d) {
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        int early_exit = 0;

        if (LEN_1D >= 4) {
            for (; i <= LEN_1D - 4; i += 4) {
                float32x4_t d_vec = __builtin_assume_aligned(d + i, 16);
                float32x4_t zero = (float32x4_t){0.0f, 0.0f, 0.0f, 0.0f};
                int32x4_t cmp = (int32x4_t)(d_vec < zero);
                if (__builtin_reduce_or(cmp)) {
                    for (int j = 0; j < 4; j++) {
                        if (d[i + j] < 0.0f) {
                            early_exit = 1;
                            break;
                        }
                    }
                    if (early_exit) break;
                }
                float32x4_t a_vec = __builtin_assume_aligned(a + i, 16);
                float32x4_t b_vec = __builtin_assume_aligned(b + i, 16);
                float32x4_t c_vec = __builtin_assume_aligned(c + i, 16);
                a_vec += b_vec * c_vec;
                *(float32x4_t*)(a + i) = a_vec;
            }
        }

        if (!early_exit) {
            for (; i < LEN_1D; i++) {
                if (d[i] < 0.0f) {
                    early_exit = 1;
                    break;
                }
                a[i] += b[i] * c[i];
            }
        }

        if (early_exit) {
            continue;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s481(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s481(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
