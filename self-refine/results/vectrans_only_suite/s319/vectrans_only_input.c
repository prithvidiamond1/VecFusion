
float s319_opt(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e)
{
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        sum = 0.;
        float sum1 = 0., sum2 = 0.;
        
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = c[i] + d[i];
            sum1 += a[i];
        }
        
        for (int i = 0; i < LEN_1D; i++) {
            b[i] = c[i] + e[i];
            sum2 += b[i];
        }
        
        sum = sum1 + sum2;
    }
    return sum;
}
