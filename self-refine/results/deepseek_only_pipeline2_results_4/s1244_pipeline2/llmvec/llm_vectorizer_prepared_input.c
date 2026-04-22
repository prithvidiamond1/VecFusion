void s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Compute all new a[i] values first
        for (int i = 0; i < LEN_1D - 1; i++) {
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
        }
        
        // Compute d[i] using new a[i] and new a[i+1] (except last element)
        for (int i = 0; i < LEN_1D - 1; i++) {
            d[i] = a[i] + a[i + 1];
        }
    }
}