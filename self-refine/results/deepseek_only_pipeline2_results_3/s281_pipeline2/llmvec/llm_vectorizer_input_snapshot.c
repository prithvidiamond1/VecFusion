void s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Process second half first (i = LEN_1D/2 to LEN_1D-1)
        // These read from a[LEN_1D-i-1] which are in the first half (unchanged)
        for (int i = LEN_1D/2; i < LEN_1D; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
        
        // Process first half (i = 0 to LEN_1D/2-1)
        // These read from a[LEN_1D-i-1] which are in the second half (now updated)
        for (int i = 0; i < LEN_1D/2; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }
}