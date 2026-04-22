void s281_opt(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First: compute all x values into a temporary array
        float x[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            x[i] = a[LEN_1D-i-1] + b[i] * c[i];
        }
        
        // Second: update a and b arrays using pre-computed x values
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = x[i] - (float)1.0;
            b[i] = x[i];
        }
    }
}
