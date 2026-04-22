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
