void s481_opt(int iterations, int LEN_1D, float* a, float *b, float *c, float* d)
{
    // Loop 1: Find the first negative element in d (cannot be vectorized due to early exit)
    int limit = LEN_1D;
    for (int i = 0; i < LEN_1D; i++) {
        if (d[i] < (float)0.) {
            limit = i;
            break;
        }
    }
    
    // Loop 2: Perform the vectorizable updates (all iterations, fully vectorizable)
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < limit; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Handle early return after first iteration if negative was found
        if (limit < LEN_1D) {
            return;
        }
    }
}
