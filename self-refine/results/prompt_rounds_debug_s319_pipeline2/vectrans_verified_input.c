float s319(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e)
{
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        sum = 0.;
        // Compute and accumulate in one pass with temporaries
        for (int i = 0; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            float tmp_b = c[i] + e[i];
            a[i] = tmp_a;
            b[i] = tmp_b;
            sum += tmp_a + tmp_b;
        }
    }
    return sum;
}