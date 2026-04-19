void s341_opt(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int j = 0;
        // First loop: count and store positive elements
        // This can be vectorized as a masked store operation
        for (int i = 0; i < LEN_1D; i++) {
            float temp = b[i];
            int mask = (temp > (float)0.);
            if (mask) {
                a[j] = temp;
                j++;
            }
        }
    }
}
