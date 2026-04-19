void s242_opt(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    float sum_scalar = s1 + s2;
    for (int nl = 0; nl < iterations/5; nl++) {
        float prev_a = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            float temp = prev_a + sum_scalar + b[i] + c[i] + d[i];
            a[i] = temp;
            prev_a = temp;
        }
    }
}
