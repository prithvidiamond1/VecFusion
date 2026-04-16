void s281_opt(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int half = LEN_1D / 2;
    for (int nl = 0; nl < iterations; nl++) {
        // Preload upper half of a into temp buffer to break the dependency for phase 1
        float temp1[half];
        for (int i = 0; i < half; i++) {
            temp1[i] = a[LEN_1D - i - 1];
        }

        // Phase 1: i in [0, half), reads from temp (original upper half values)
        for (int i = 0; i < half; i++) {
            float x = temp1[i] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }

        // Preload lower half of a (already updated in phase 1) into temp buffer
        // a[LEN_1D-i-1] for i in [half, LEN_1D) maps to a[0..half-1]
        float temp2[half];
        for (int i = 0; i < half; i++) {
            temp2[i] = a[i];
        }

        // Phase 2: i in [half, LEN_1D), reads from temp2 (no dependency on a[])
        for (int i = half; i < LEN_1D; i++) {
            float x = temp2[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }
}
