void s211_opt(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original b[i+1] values before any updates
        float b_plus1[LEN_1D];
        for (int i = 1; i < LEN_1D-1; i++) {
            b_plus1[i] = b[i+1];
        }
        
        // Compute b[i] using stored original b[i+1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b_plus1[i] - e[i] * d[i];
        }
        
        // Now compute a[i] using updated b[i-1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}
