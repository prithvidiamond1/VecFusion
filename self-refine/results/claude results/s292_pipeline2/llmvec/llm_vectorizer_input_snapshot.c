void s292(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Handle wrap-around cases for i=0 and i=1
        a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float)0.333;
        a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float)0.333;
        // For i>=2, access is sequential: b[i], b[i-1], b[i-2]
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float)0.333;
        }
    }
}