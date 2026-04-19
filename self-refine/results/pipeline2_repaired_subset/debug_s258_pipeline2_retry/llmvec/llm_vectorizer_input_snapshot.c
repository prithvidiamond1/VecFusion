void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    float s_array[256];
    float d_sq[256];
    
    // Precompute d[i]*d[i] once per outer iteration
    for (int i = 0; i < 256; ++i) {
        d_sq[i] = d[i] * d[i];
    }
    
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        
        // First loop: compute s values using precomputed d_sq
        // This can now be vectorized with masked assignment
        for (int i = 0; i < 256; ++i) {
            // Branchless conditional update using comparison result as mask
            float mask = (a[i] > 0.0f) ? 1.0f : 0.0f;
            s = mask * d_sq[i] + (1.0f - mask) * s;
            s_array[i] = s;
        }
        
        // Second loop: compute b and e using stored s values
        // This loop is fully vectorizable
        for (int i = 0; i < 256; ++i) {
            float temp_s = s_array[i];
            b[i] = temp_s * c[i] + d[i];
            e[i] = (temp_s + 1.0f) * aa[0][i];
        }
    }
}