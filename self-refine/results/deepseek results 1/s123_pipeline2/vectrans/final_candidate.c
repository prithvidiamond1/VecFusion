void s123_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int n = LEN_1D / 2;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Precompute d[i]*e[i] for all i (vectorizable)
        float de[n];
        for (int i = 0; i < n; i++) {
            de[i] = d[i] * e[i];
        }
        
        // Compute b[i] + de[i] for all i (vectorizable)
        float b_plus_de[n];
        for (int i = 0; i < n; i++) {
            b_plus_de[i] = b[i] + de[i];
        }
        
        // Compute c[i] + de[i] and condition mask (vectorizable)
        float c_plus_de[n];
        int mask[n];
        for (int i = 0; i < n; i++) {
            mask[i] = (c[i] > 0.0f);
            c_plus_de[i] = c[i] + de[i];
        }
        
        // Precompute write indices using prefix sum (vectorizable)
        int indices[n];
        indices[0] = 0;
        for (int i = 1; i < n; i++) {
            indices[i] = indices[i-1] + 1 + mask[i-1];
        }
        
        // First write pass: always write b_plus_de (vectorizable)
        for (int i = 0; i < n; i++) {
            a[indices[i]] = b_plus_de[i];
        }
        
        // Second write pass: conditional writes for c_plus_de (vectorizable)
        for (int i = 0; i < n; i++) {
            if (mask[i]) {
                a[indices[i] + 1] = c_plus_de[i];
            }
        }
    }
}
