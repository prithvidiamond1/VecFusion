void s323_opt(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Compute prefix sum for b first
        float b_prev = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b_prev + c[i] * (d[i] + e[i]);
            b_prev = b[i];
        }
        
        // Now compute a in a separate vectorizable loop
        a[1] = b[0] + c[1] * d[1];
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}
