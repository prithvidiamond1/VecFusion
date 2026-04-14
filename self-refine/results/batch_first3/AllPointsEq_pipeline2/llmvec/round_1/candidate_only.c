#include <stdbool.h>
#include <stdint.h>

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 0) return true;

    int first = pts[0];

    // Use GCC/Clang vector extensions for SIMD-style processing
    typedef int v8si __attribute__((vector_size(32)));  // 8 x int32 = 256-bit

    int i = 1;
    int result = 1;

    // Vectorized portion: process 8 ints at a time
    if (count - 1 >= 8) {
        v8si vfirst = {first, first, first, first, first, first, first, first};
        v8si vresult = {1, 1, 1, 1, 1, 1, 1, 1};

        for (; i + 7 < count; i += 8) {
            v8si vdata;
            __builtin_memcpy(&vdata, &pts[i], sizeof(v8si));
            // Compare: produces -1 (all bits set) where equal, 0 where not
            v8si vcmp = (vdata == vfirst);
            vresult &= vcmp;
        }

        // Reduce vresult: check all lanes are non-zero (i.e., -1)
        int tmp[8];
        __builtin_memcpy(tmp, &vresult, sizeof(tmp));
        for (int j = 0; j < 8; j++) {
            result &= (tmp[j] != 0);
        }
    }

    // Scalar cleanup tail
    for (; i < count; i++) {
        result &= (first == pts[i]);
    }

    return (bool)result;
}
