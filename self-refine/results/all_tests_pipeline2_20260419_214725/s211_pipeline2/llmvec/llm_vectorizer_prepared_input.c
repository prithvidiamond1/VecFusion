void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store b[i+1] values before modification to break dependency
        float b_next[LEN_1D];
        for (int i = 1; i < LEN_1D-1; i++) {
            b_next[i] = b[i+1];
        }
        
        // Compute all b[i] updates using stored b[i+1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b_next[i] - e[i] * d[i];
        }
        
        // Compute all a[i] using updated b[i-1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}