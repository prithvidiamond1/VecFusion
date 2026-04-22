
void s1244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Loop distribution: compute all new a[i] first
        for (int i = 0; i < LEN_1D-1; i++) {
            float bi = b[i];
            float ci = c[i];
            a[i] = bi + ci * ci + bi * bi + ci;
        }
        // Then compute d[i] using new a[i] and new a[i+1]
        for (int i = 0; i < LEN_1D-1; i++) {
            d[i] = a[i] + a[i+1];
        }
    }
}
