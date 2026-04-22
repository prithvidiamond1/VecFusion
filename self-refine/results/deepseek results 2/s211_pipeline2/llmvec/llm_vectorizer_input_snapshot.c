void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First compute all b[i] updates using original b[i+1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            float temp_b_next = b[i+1];
            b[i] = temp_b_next - e[i] * d[i];
        }
        
        // Then compute all a[i] using updated b[i-1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}