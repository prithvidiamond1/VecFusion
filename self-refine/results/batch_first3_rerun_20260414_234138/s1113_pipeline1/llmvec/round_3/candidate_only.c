#include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    typedef float float8 __attribute__((vector_size(32)));
    for (int nl = 0; nl < 2*iterations; nl++) {
        int mid = LEN_1D / 2;

        /* Phase 1: indices [0, mid) using original a[mid] */
        float scalar_val = a[mid];
        float8 vscalar = {scalar_val, scalar_val, scalar_val, scalar_val,
                          scalar_val, scalar_val, scalar_val, scalar_val};
        int i = 0;
        for (; i <= mid - 8; i += 8) {
            float8 vb;
            __builtin_memcpy(&vb, b + i, 32);
            float8 va = vscalar + vb;
            __builtin_memcpy(a + i, &va, 32);
        }
        for (; i < mid; i++) {
            a[i] = scalar_val + b[i];
        }

        /* Phase 2: index == mid, overwrites a[mid] */
        a[mid] = scalar_val + b[mid];

        /* Re-capture scalar after a[mid] is updated */
        scalar_val = a[mid];
        float8 vscalar2 = {scalar_val, scalar_val, scalar_val, scalar_val,
                           scalar_val, scalar_val, scalar_val, scalar_val};

        /* Phase 3: indices [mid+1, LEN_1D) using new a[mid] */
        i = mid + 1;
        for (; i <= LEN_1D - 8; i += 8) {
            float8 vb;
            __builtin_memcpy(&vb, b + i, 32);
            float8 va = vscalar2 + vb;
            __builtin_memcpy(a + i, &va, 32);
        }
        for (; i < LEN_1D; i++) {
            a[i] = scalar_val + b[i];
        }
    }
}
