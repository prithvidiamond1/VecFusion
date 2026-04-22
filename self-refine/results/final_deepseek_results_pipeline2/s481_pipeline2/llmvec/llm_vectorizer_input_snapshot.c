void s481_opt(int iterations, int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int early_exit = 0;
        int exit_index = LEN_1D;
        
        // First check if any d[i] < 0 and find the first occurrence
        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < 0.0f) {
                early_exit = 1;
                exit_index = i;
                break;
            }
        }
        
        // Process elements before exit_index (if any)
        if (early_exit) {
            for (int i = 0; i < exit_index; i++) {
                a[i] += b[i] * c[i];
            }
            return;
        }
        
        // No early exit - process all elements (vectorizable, no branches)
        for (int i = 0; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}
