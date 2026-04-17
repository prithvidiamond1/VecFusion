float s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        float x0 = a[0];
        float x1 = a[0];
        float x2 = a[0];
        float x3 = a[0];

        int i;
        for (i = 1; i < LEN_1D - 3; i += 4) {
            x0 = x0 < a[i]   ? x0 : a[i];
            x1 = x1 < a[i+1] ? x1 : a[i+1];
            x2 = x2 < a[i+2] ? x2 : a[i+2];
            x3 = x3 < a[i+3] ? x3 : a[i+3];
        }
        for (; i < LEN_1D; i++) {
            x0 = x0 < a[i] ? x0 : a[i];
        }
        x0 = x0 < x1 ? x0 : x1;
        x2 = x2 < x3 ? x2 : x3;
        x = x0 < x2 ? x0 : x2;
    }
    return x;
}