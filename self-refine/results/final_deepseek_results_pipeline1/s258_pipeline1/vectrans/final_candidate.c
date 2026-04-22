void s258_opt(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        float s_values[256];
        
        // First loop: compute s for each iteration (reduction pattern)
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.0f) {
                s = d[i] * d[i];
            }
            s_values[i] = s;
        }
        
        // Second loop: use computed s values (vectorizable)
        for (int i = 0; i < 256; ++i) {
            b[i] = s_values[i] * c[i] + d[i];
            e[i] = (s_values[i] + 1.0f) * aa[0][i];
        }
    }
}
