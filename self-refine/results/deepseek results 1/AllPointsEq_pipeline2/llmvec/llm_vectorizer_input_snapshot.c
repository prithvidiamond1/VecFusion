bool AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;
    
    const int first = pts[0];
    int i = 1;
    
    // Vectorization-friendly main loop: accumulate mismatches
    int any_mismatch = 0;
    for (; i + 3 < count; i += 4) {
        any_mismatch |= (first != pts[i]);
        any_mismatch |= (first != pts[i+1]);
        any_mismatch |= (first != pts[i+2]);
        any_mismatch |= (first != pts[i+3]);
    }
    
    // Early exit after vectorized portion
    if (any_mismatch) return false;
    
    // Cleanup loop for remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }
    
    return true;
}