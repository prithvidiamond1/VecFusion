#include <stdbool.h>
#include <stdint.h>

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int val = pts[0];

    // Use GCC/Clang vector extensions for SIMD-style processing
    typedef int v8si __attribute__((vector_size(32)));  // 8 x int32 = 256-bit

    int i = 1;
    int all_eq = 1;

    // Vector phase: process 8 ints at a time
    if (count - 1 >= 8) {
        v8si vval = {val, val, val, val, val, val, val, val};
        v8si vacc = {1, 1, 1, 1, 1, 1, 1, 1};

        for (; i + 7 < count; i += 8) {
            v8si vpts;
            __builtin_memcpy(&vpts, &pts[i], sizeof(v8si));
            // Compare: produces -1 (all bits set) for equal, 0 for not equal
            v8si cmp = (vpts == vval);
            vacc &= cmp;
        }

        // Reduce vacc: check all lanes are non-zero (i.e., -1)
        int *acc_arr = (int *)&vacc;
        for (int j = 0; j < 8; j++) {
            all_eq &= (acc_arr[j] != 0);
        }
    }

    // Scalar cleanup tail
    for (; i < count; i++) {
        all_eq &= (val == pts[i]);
    }

    return (bool)all_eq;
}
