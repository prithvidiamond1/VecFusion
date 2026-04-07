#include <stdint.h>

typedef int v8si __attribute__((vector_size(32)));

void vectorized_saxpy_like(const int *a, const int *b, int *out, int n) {
    int i = 0;
    int vec_len = 8; // 8 ints in 256-bit vector

    // Process 8 elements at a time using GCC/Clang vector extensions
    for (; i <= n - vec_len; i += vec_len) {
        v8si va, vb, vout;

        // Load 8 ints from a and b
        __builtin_memcpy(&va, a + i, sizeof(v8si));
        __builtin_memcpy(&vb, b + i, sizeof(v8si));

        // Compute a[i] + 2 * b[i]
        vout = va + 2 * vb;

        // Store result
        __builtin_memcpy(out + i, &vout, sizeof(v8si));
    }

    // Scalar cleanup for remaining elements
    for (; i < n; ++i) {
        out[i] = a[i] + 2 * b[i];
    }
}
