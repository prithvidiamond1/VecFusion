void s258_opt(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s_arr[256];
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute s values for each index (resolve loop-carried dependency)
        float s = 0.0f;
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.0f) {
                s = d[i] * d[i];
            }
            s_arr[i] = s;
        }
        // Second pass: compute b[i] - now vectorizable
        for (int i = 0; i < 256; ++i) {
            b[i] = s_arr[i] * c[i] + d[i];
        }
        // Third pass: compute e[i] - now vectorizable
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_arr[i] + 1.0f) * aa[0][i];
        }
    }
}
