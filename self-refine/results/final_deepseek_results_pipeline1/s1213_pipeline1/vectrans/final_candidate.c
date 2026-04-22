void s1213_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute all b[i] values using original a[i+1]
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = a[i+1] * d[i];
        }
        
        // Second loop: compute all a[i] values using updated b[i-1]
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i];
        }
    }
}
