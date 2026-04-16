float s314(int iterations, int LEN_1D, float* a)
{
    float x = a[0];
    for (int i = 1; i < LEN_1D; i++) {
        float ai = a[i];
        x = ai > x ? ai : x;
    }
    return x;
}