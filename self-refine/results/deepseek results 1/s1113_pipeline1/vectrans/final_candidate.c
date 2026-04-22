void s1113_opt(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;
    float a_mid, new_mid;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Store original mid value
        a_mid = a[mid];
        
        // First half: i = 0 to mid-1 (uses original a[mid])
        for (int i = 0; i < mid; i++) {
            a[i] = a_mid + b[i];
        }
        
        // Compute new mid value in temporary
        new_mid = a_mid + b[mid];
        
        // Second half: i = mid+1 to LEN_1D-1 (uses computed new_mid)
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = new_mid + b[i];
        }
        
        // Finally update the actual mid element
        a[mid] = new_mid;
    }
}
