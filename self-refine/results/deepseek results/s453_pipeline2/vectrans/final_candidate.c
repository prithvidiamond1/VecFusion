void s453_opt(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations*2; nl++) {
        // First loop: compute s values without recurrence
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = 2.0f * (i + 1);
        }
        // Second loop: multiply with b[i]
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = a[i] * b[i];
        }
    }
}
