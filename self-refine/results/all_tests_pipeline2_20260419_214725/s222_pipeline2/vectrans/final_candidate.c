void s222_opt(int iterations, int LEN_1D, float*a,float*b,float*c, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original e values to break loop-carried dependency
        float e_original[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            e_original[i] = e[i];
        }
        
        // Update e[i] using original e[i-1] values (sequential)
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e_original[i-1] * e_original[i-1];
        }
        
        // Independent a[i] operations (can be vectorized)
        for (int i = 1; i < LEN_1D; i++) {
            float tmp = b[i] * c[i];
            a[i] += tmp;
            a[i] -= tmp;
        }
    }
}
