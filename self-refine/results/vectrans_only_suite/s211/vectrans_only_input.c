
void s211_opt(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Split the loop into two phases to break the loop-carried dependency
        // Phase 1: Compute all b[i] updates first using original b[i+1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b[i+1] - e[i] * d[i];
        }
        
        // Phase 2: Compute all a[i] using the updated b[i-1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}
