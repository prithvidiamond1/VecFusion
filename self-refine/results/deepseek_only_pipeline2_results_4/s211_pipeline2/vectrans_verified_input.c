void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Reorder instructions to break dependency chain
        // First compute all b[i] updates using original b[i+1]
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b[i + 1] - e[i] * d[i];
        }
        
        // Then compute all a[i] using the newly updated b[i-1]
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
        }
    }
}