void s321_opt(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original values to break loop-carried dependency
        float prev = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            float current = a[i];
            a[i] = current + prev * b[i];
            prev = current;
        }
    }
}
