void s1244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original a[i+1] values before overwriting a[i]
        for (int i = 0; i < LEN_1D-1; i++) {
            float bi = b[i];
            float ci = c[i];
            float a_next = a[i+1];  // Store original a[i+1] before computing new a[i]
            a[i] = bi + ci * ci + bi * bi + ci;
            d[i] = a[i] + a_next;   // Use stored original a[i+1]
        }
    }
}
