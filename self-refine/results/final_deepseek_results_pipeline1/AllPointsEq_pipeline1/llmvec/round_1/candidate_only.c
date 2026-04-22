#include <stdbool.h>
#include <stdint.h>

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;
    
    int first = pts[0];
    int i = 1;
    
    // Process 4 elements at a time using int vectors
    for (; i + 3 < count; i += 4) {
        int v0 = pts[i];
        int v1 = pts[i+1];
        int v2 = pts[i+2];
        int v3 = pts[i+3];
        if (first != v0 || first != v1 || first != v2 || first != v3) {
            return false;
        }
    }
    
    // Scalar cleanup for remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }
    
    return true;
}
