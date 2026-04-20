bool AllPointsEq_opt(const int pts[], int count) {
    if (count <= 1) return true;
    
    int first = pts[0];
    int all_eq = 1;
    
    // Process elements in blocks
    int i = 1;
    for (; i + 3 < count; i += 4) {
        int eq1 = (first == pts[i]);
        int eq2 = (first == pts[i + 1]);
        int eq3 = (first == pts[i + 2]);
        int eq4 = (first == pts[i + 3]);
        
        int block_eq = eq1 & eq2 & eq3 & eq4;
        all_eq &= block_eq;
        
        // Early exit if mismatch found
        if (!all_eq) return false;
    }
    
    // Handle remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }
    
    return true;
}
