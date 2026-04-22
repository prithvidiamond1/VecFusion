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
