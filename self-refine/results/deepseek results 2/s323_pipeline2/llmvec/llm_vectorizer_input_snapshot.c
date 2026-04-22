void s323(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Precompute c*d and c*e into temporary arrays
        float cd[LEN_1D];
        float ce[LEN_1D];
        for (int i = 1; i < LEN_1D; i++) {
            cd[i] = c[i] * d[i];
            ce[i] = c[i] * e[i];
        }
        
        // Compute b[i] = b[i-1] + (cd[i] + ce[i]) using parallel prefix sum
        float b_temp[LEN_1D];
        b_temp[0] = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            b_temp[i] = cd[i] + ce[i];
        }
        
        // Serial prefix sum for recurrence (vectorizable with compiler support for reductions)
        for (int i = 1; i < LEN_1D; i++) {
            b_temp[i] = b_temp[i-1] + b_temp[i];
        }
        
        // Compute a[i] = b[i-1] + cd[i] (now vectorizable)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = b_temp[i-1] + cd[i];
        }
        
        // Copy b_temp back to b (vectorizable)
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b_temp[i];
        }
    }
}