float s319(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e)
{
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        sum = 0.;
        float sum_a = 0.;
        float sum_b = 0.;
        
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = c[i] + d[i];
            sum_a += a[i];
        }
        
        for (int i = 0; i < LEN_1D; i++) {
            b[i] = c[i] + e[i];
            sum_b += b[i];
        }
        
        sum = sum_a + sum_b;
    }
    return sum;
}
