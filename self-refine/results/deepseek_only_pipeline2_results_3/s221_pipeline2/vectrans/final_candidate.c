void s221_opt(int iterations, int LEN_1D, float*a,float*b,float*c,float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original b[0] value for prefix sum initialization
        float b0_original = b[0];
        
        // First compute all a[i] updates (fully independent)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }
        
        // Compute prefix sum for b[i] using original b[0]
        // This preserves the exact recurrence pattern b[i] = b[i-1] + a[i] + d[i]
        b[1] = b0_original + a[1] + d[1];
        for (int i = 2; i < LEN_1D; i++) {
            b[i] = b[i-1] + a[i] + d[i];
        }
    }
}
