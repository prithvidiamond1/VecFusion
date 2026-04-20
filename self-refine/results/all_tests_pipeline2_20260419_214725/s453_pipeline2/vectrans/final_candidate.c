void s453_opt(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations*2; nl++) {
        // Remove the recurrence by computing s as 2.0*(i+1) directly
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = (2.0f * (i + 1)) * b[i];
        }
    }
}
