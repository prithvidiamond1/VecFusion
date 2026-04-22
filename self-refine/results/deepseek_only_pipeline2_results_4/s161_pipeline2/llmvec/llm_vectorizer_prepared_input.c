void s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // First pass: compute all c[i+1] updates for b[i] < 0
        // Use original a[i] values before any updates in this iteration
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] < (float)0.) {
                c[i+1] = a[i] + d[i] * d[i];
            }
        }
        
        // Second pass: compute all a[i] updates for b[i] >= 0
        // Use updated c[i] values from first pass when needed
        for (int i = 0; i < LEN_1D; ++i) {
            if (b[i] >= (float)0.) {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}