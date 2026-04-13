#include <stdint.h>
#include <string.h>

void vectorized_AllPointsEq(const int a[], const int b[], int *out_vector, int n) {
    typedef int v8si __attribute__((vector_size(32)));

    int i = 0;
    int simd_end = (n / 8) * 8;

    for (; i < simd_end; i += 8) {
        v8si va, vb;
        memcpy(&va, &a[i], sizeof(v8si));
        memcpy(&vb, &b[i], sizeof(v8si));
        v8si cmp = (va == vb);
        /* cmp lanes are -1 (all bits set) when equal, 0 when not equal */
        out_vector[i + 0] = cmp[0] ? 1 : 0;
        out_vector[i + 1] = cmp[1] ? 1 : 0;
        out_vector[i + 2] = cmp[2] ? 1 : 0;
        out_vector[i + 3] = cmp[3] ? 1 : 0;
        out_vector[i + 4] = cmp[4] ? 1 : 0;
        out_vector[i + 5] = cmp[5] ? 1 : 0;
        out_vector[i + 6] = cmp[6] ? 1 : 0;
        out_vector[i + 7] = cmp[7] ? 1 : 0;
    }

    /* Scalar tail */
    for (; i < n; ++i) {
        out_vector[i] = (a[i] == b[i]) ? 1 : 0;
    }
}
