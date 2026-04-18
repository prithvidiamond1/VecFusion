void s481(int iterations,int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int exit_point = LEN_1D;
        
        // Vector-friendly scan: find first negative without early exit in inner loop
        int i;
        for (i = 0; i < LEN_1D; i++) {
            if (d[i] < (float)0.) {
                exit_point = i;
                break;
            }
        }
        
        // Vectorized computation loop
        for (int j = 0; j < exit_point; j++) {
            a[j] += b[j] * c[j];
        }
        
        // Early return after computation if negative found
        if (exit_point < LEN_1D) {
            return;
        }
    }
}