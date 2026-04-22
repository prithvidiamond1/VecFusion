void s342(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute prefix sum of conditions into a temporary array
        int prefix[LEN_1D];
        int count = -1;
        for (int i = 0; i < LEN_1D; i++) {
            int cond = (a[i] > (float)0.);
            count += cond;
            prefix[i] = cond ? count : -1;
        }
        
        // Second loop: vectorizable assignment using precomputed indices
        for (int i = 0; i < LEN_1D; i++) {
            int idx = prefix[i];
            if (idx != -1) {
                a[i] = b[idx];
            }
        }
    }
}