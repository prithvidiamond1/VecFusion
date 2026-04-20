void s322_opt(int iterations, int LEN_1D, float* a, float* b, float* c) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original values for the first two elements
        float a0 = a[0];
        float a1 = a[1];
        
        // Process elements with dependency chain
        for (int i = 2; i < LEN_1D; i++) {
            // Compute new value using updated dependencies
            float new_val = a[i] + a1 * b[i] + a0 * c[i];
            
            // Shift dependencies for next iteration
            a0 = a1;       // a[i-2] becomes old a[i-1]
            a1 = new_val;  // a[i-1] becomes updated a[i]
            
            // Update array with new value
            a[i] = new_val;
        }
    }
}
