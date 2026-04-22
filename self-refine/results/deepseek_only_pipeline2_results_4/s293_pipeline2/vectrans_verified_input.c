void s293(int iterations,int LEN_1D, float* a)
{
    float temp;
    for (int nl = 0; nl < 4*iterations; nl++) {
        temp = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = temp;
        }
    }
}