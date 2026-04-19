void s481_opt(int iterations,int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: check condition and compute partial updates
        int should_return = 0;
        int last_safe = 0;
        
        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < (float)0.) {
                should_return = 1;
                last_safe = i;
                break;
            }
        }
        
        // Update only up to last_safe (or all if no negative found)
        int limit = should_return ? last_safe : LEN_1D;
        for (int i = 0; i < limit; i++) {
            a[i] += b[i] * c[i];
        }
        
        if (should_return) return;
    }
}
