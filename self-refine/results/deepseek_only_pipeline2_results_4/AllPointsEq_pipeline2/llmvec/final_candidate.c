#include <stdbool.h>
#include <stddef.h>

typedef int v4si __attribute__((vector_size(16)));

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;
    
    int first = pts[0];
    int i = 1;
    int found_mismatch = 0;
    
    // Vectorized main loop
    for (; i + 3 < count; i += 4) {
        // Load 4 consecutive elements
        v4si chunk = *(const v4si*)(&pts[i]);
        // Broadcast first to vector
        v4si first_vec = (v4si){first, first, first, first};
        // Compare for inequality
        v4si cmp_vec = first_vec != chunk;
        // Check if any element is non-zero
        int any_mismatch = (cmp_vec[0] | cmp_vec[1] | cmp_vec[2] | cmp_vec[3]);
        found_mismatch |= any_mismatch;
    }
    
    if (found_mismatch) return false;
    
    // Scalar tail
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }
    
    return true;
}
