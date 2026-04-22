void s261_opt(int iterations,int LEN_1D, float* a,float* b,float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Loop distribution: separate c update from a update
        // First compute all new c values into a temporary array
        float c_new[LEN_1D];
        for (int i = 1; i < LEN_1D; ++i) {
            c_new[i] = c[i] * d[i];
        }
        
        // Then compute a values using original c values
        for (int i = 1; i < LEN_1D; ++i) {
            float t = a[i] + b[i];
            a[i] = t + c[i-1];
        }
        
        // Copy back c values
        for (int i = 1; i < LEN_1D; ++i) {
            c[i] = c_new[i];
        }
    }
}
