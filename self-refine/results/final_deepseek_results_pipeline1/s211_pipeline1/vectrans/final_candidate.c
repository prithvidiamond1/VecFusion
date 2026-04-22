void s211_opt(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original b[i+1] values needed for b[i] computation
        float* orig_b_next = (float*)malloc((LEN_1D - 1) * sizeof(float));
        for (int i = 1; i < LEN_1D - 1; i++) {
            orig_b_next[i] = b[i + 1];
        }
        
        // First compute b[i] using original b[i+1]
        for (int i = 1; i < LEN_1D - 1; i++) {
            b[i] = orig_b_next[i] - e[i] * d[i];
        }
        
        // Then compute a[i] using the updated b[i-1]
        for (int i = 1; i < LEN_1D - 1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
        }
        
        free(orig_b_next);
    }
}
