void s221_opt(int iterations, int LEN_1D, float*a,float*b,float*c,float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Loop distribution: separate independent a[i] update from dependent b[i] recurrence
        // First loop: independent parallel computation
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }
        // Second loop: sequential recurrence preserved
        float b_prev = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            float temp = b_prev + a[i] + d[i];
            b_prev = temp;
            b[i] = temp;
        }
    }
}
