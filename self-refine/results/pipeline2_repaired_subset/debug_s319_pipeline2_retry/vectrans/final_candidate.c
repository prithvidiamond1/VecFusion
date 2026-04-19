float s319_opt(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e)
{
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        sum = 0.;
        for (int i = 0; i < LEN_1D; i++) {
            float ai = c[i] + d[i];
            float bi = c[i] + e[i];
            a[i] = ai;
            b[i] = bi;
            sum += ai;
            sum += bi;
        }
    }
    return sum;
}
