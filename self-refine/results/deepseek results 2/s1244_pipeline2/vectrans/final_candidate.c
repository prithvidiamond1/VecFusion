void s1244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First compute all new a[i] values into a temporary array
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
        }
        
        // Then compute d[i] using consecutive a[i] values
        for (int i = 0; i < LEN_1D-1; i++) {
            d[i] = a[i] + a[i+1];
        }
    }
}
