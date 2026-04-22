void s482_opt(int iterations, int LEN_1D, float* a, float *b, float *c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Find the break point first  
        int break_point = LEN_1D;
        for (int i = 0; i < LEN_1D - 1; i++) {
            if (c[i] > b[i]) {
                break_point = i;
                break;
            }
        }
        
        // Vectorizable prefix: no break condition to check  
        for (int i = 0; i < break_point; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Handle the break iteration  
        if (break_point < LEN_1D) {
            a[break_point] += b[break_point] * c[break_point];
        }
    }
}
