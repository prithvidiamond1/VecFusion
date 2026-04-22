float s312(int iterations,int LEN_1D, float* a)
{
    float prod = (float)1.;
    for (int nl = 0; nl < 10*iterations; nl++) {
        float temp_prod = (float)1.;
        for (int i = 0; i < LEN_1D; i++) {
            temp_prod *= a[i];
        }
        prod = temp_prod;
    }
    return prod;
}
