bool AllPointsEq_opt(const int pts[], int count) {
    if (count <= 1) return true;
    
    int first = pts[0];
    int i = 1;
    
    // Loop distribution: separate comparison from early exit
    // First, process in blocks without branching
    int remaining = count - 1;
    int block_size = 4;
    int vectorizable_iters = remaining / block_size;
    int vectorized_end = vectorizable_iters * block_size + 1;
    
    // Remove branching logic using arithmetic masking
    int all_equal = 1;  // Start assuming all equal
    
    // Main vectorizable loop - no early exit inside
    // Reorder: cluster memory accesses first, then compute comparisons
    for (; i < vectorized_end; i += block_size) {
        // Cluster similar memory accesses
        int val0, val1, val2, val3;
        val0 = pts[i];
        val1 = pts[i+1];
        val2 = pts[i+2];
        val3 = pts[i+3];
        
        // Compute equality mask without branching
        int eq0 = (first == val0);
        int eq1 = (first == val1);
        int eq2 = (first == val2);
        int eq3 = (first == val3);
        
        // Accumulate result using pure arithmetic reduction
        // This helps compiler recognize reduction pattern
        int block_equal = eq0 & eq1 & eq2 & eq3;
        all_equal = all_equal & block_equal;
    }
    
    // Early exit check after vector loop
    if (!all_equal) return false;
    
    // Cleanup loop (non-vectorizable tail)
    for (; i < count; ++i) {
        if (first != pts[i]) return false;
    }
    
    return true;
}
