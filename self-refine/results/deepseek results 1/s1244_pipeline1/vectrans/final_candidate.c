void s1244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original a[i+1] values before overwriting them
        float a_next = a[0];
        for (int i = 0; i < LEN_1D-1; i++) {
            float bi = b[i];
            float ci = c[i];
            float a_current = bi + ci * ci + bi * bi + ci;
            
            // Use stored original a[i+1] for d[i]
            d[i] = a_current + a_next;
            
            // Update a[i] and prepare a_next for next iteration
            a[i] = a_current;
            a_next = a[i+1];
        }
    }
}
