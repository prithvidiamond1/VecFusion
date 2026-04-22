float s319_opt(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e)
{
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        sum = 0.;
        
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = c[i] + d[i];
            b[i] = c[i] + e[i];
        }
        
        for (int i = 0; i < LEN_1D; i++) {
            sum += a[i];
            sum += b[i];
        }
    }
    return sum;
}
