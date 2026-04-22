void s481(int iterations,int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: check for negative values without computation
        int should_return = 0;
        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < (float)0.) {
                should_return = 1;
                break;
            }
        }
        if (should_return) return;
        
        // Second loop: pure computation without branching
        for (int i = 0; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}