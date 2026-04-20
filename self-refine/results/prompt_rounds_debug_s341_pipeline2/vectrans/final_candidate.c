void s341_opt(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        for (int i = 0; i < LEN_1D; i++) {
            float temp = b[i];
            int mask = temp > (float)0.;
            a[write_idx] = temp;
            write_idx += mask;
        }
    }
}
