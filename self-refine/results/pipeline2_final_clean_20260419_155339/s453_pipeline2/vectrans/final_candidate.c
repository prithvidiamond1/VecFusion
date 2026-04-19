void s453_opt(int iterations, int LEN_1D, float* a, float *b)
{
    float s;
    for (int nl = 0; nl < iterations*2; nl++) {
        s = 0.;
        // Separate the accumulation of s from the multiplication
        for (int i = 0; i < LEN_1D; i++) {
            s += (float)2.;
            // Store intermediate s values in a[i] temporarily
            a[i] = s;
        }
        // Independent multiplication loop (easily vectorizable)
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = a[i] * b[i];
        }
    }
}
