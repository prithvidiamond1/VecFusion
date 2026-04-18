float s315(int iterations, int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x;
    int index;

    for (int nl = 0; nl < iterations; nl++) {
        // Phase 1: Vectorizable max reduction (value only)
        float max_val = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            float val = a[i];
            max_val = (val > max_val) ? val : max_val;
        }
        
        // Phase 2: Find index of max_val (can be vectorized with compare)
        int max_idx = 0;
        for (int i = 0; i < LEN_1D; ++i) {
            max_idx = (a[i] == max_val) ? i : max_idx;
        }
        
        x = max_val;
        index = max_idx;
    }
    return index + x + 1;
}