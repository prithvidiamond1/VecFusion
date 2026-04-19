bool AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;
    
    int first = pts[0];
    int i = 1;
    int found_mismatch = 0;
    
    // Main vectorizable loop without early exit
    for (; i + 3 < count; i += 4) {
        // Independent comparisons - compute mismatch mask
        int cmp0 = (first != pts[i]);
        int cmp1 = (first != pts[i + 1]);
        int cmp2 = (first != pts[i + 2]);
        int cmp3 = (first != pts[i + 3]);
        
        // Accumulate results without branching
        found_mismatch |= (cmp0 | cmp1 | cmp2 | cmp3);
    }
    
    // Early exit after vectorized portion
    if (found_mismatch) return false;
    
    // Handle remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }
    
    return true;
}