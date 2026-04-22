void s322_opt(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        float t1 = a[0];
        float t2 = a[1];
        
        for (int i = 2; i < LEN_1D; i++) {
            float result = a[i] + t2 * b[i] + t1 * c[i];
            t1 = t2;
            t2 = result;
            a[i] = result;
        }
    }
}
