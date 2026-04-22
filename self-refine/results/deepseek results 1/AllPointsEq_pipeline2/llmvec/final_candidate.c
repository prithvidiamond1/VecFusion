#include <stdbool.h>
#include <stdint.h>

typedef int v4si __attribute__((vector_size(16)));

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    const int first = pts[0];
    int i = 1;

    // Build a vector of 'first' repeated 4 times
    v4si vfirst = {first, first, first, first};
    v4si vmismatch = {0, 0, 0, 0};

    // Vectorized main loop: process 4 elements at a time
    for (; i + 3 < count; i += 4) {
        v4si vpts;
        __builtin_memcpy(&vpts, &pts[i], sizeof(v4si));
        // Compare: result is -1 (all bits set) where equal, 0 where not equal
        v4si vcmp = (vfirst != vpts);
        vmismatch |= vcmp;
    }

    // Check if any mismatch was found in vectorized portion
    // vmismatch elements are non-zero if there was a mismatch
    if (vmismatch[0] | vmismatch[1] | vmismatch[2] | vmismatch[3]) {
        return false;
    }

    // Scalar cleanup tail for remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }

    return true;
}
