float s314_opt(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            float current = a[i];
            x = (current > x) ? current : x;
        }
    }
    return x;
}
