void s482_opt(int iterations, int LEN_1D, float* a, float *b, float *c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: find break point without vectorization
        int break_index = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_index = i;
                break;
            }
        }
        
        // Second loop: vectorizable computation up to break point
        for (int i = 0; i < break_index; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Handle the break point iteration if it exists
        if (break_index < LEN_1D) {
            a[break_index] += b[break_index] * c[break_index];
        }
    }
}
