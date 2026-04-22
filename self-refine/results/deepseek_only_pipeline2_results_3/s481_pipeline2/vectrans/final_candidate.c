void s481_opt(int iterations,int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int exit_flag = 0;
        int exit_index = LEN_1D;
        
        // First loop: check condition and find first negative
        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < (float)0.) {
                exit_flag = 1;
                exit_index = i;
                break;
            }
        }
        
        // Second loop: compute only up to exit_index
        for (int i = 0; i < exit_index; i++) {
            a[i] += b[i] * c[i];
        }
        
        if (exit_flag) {
            return;
        }
    }
}
