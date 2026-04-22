void s453(int iterations, int LEN_1D, float* a, float *b)
{
    float s;
    for (int nl = 0; nl < iterations * 2; nl++) {
        // Compute s directly from loop index to break the dependency chain
        for (int i = 0; i < LEN_1D; i++) {
            s = (float)(2.0 * (i + 1));
            a[i] = s * b[i];
        }
    }
}
