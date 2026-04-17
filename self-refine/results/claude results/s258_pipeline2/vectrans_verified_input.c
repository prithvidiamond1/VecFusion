void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        // First pass: compute s values and store them
        float s_arr[256];
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.) {
                s = d[i] * d[i];
            }
            s_arr[i] = s;
        }
        // Second pass: compute b[i] - vectorizable
        for (int i = 0; i < 256; ++i) {
            b[i] = s_arr[i] * c[i] + d[i];
        }
        // Third pass: compute e[i] - vectorizable
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_arr[i] + (float)1.) * aa[0][i];
        }
    }
}