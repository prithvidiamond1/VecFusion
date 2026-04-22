
void s481_opt(int iterations, int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Early exit detection with partial updates preserved
        int exit_index = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < 0.0f) {
                exit_index = i;
                break;
            }
        }
        
        // Vectorizable computation loop up to exit_index
        for (int i = 0; i < exit_index; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Return if early exit was triggered
        if (exit_index < LEN_1D) {
            return;
        }
    }
}
