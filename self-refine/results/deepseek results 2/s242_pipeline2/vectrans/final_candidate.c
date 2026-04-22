void s242_opt(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    float s_sum = s1 + s2;
    int outer_iters = iterations / 5;
    
    for (int nl = 0; nl < outer_iters; nl++) {
        float prev_a = a[0];
        
        // Temporary array to store independent sums
        float* tmp = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        
        // Vectorizable loop: compute independent sums
        for (int i = 1; i < LEN_1D; ++i) {
            tmp[i] = s_sum + b[i] + c[i] + d[i];
        }
        
        // Clean recurrence: only depends on previous a[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            prev_a = prev_a + tmp[i];
            a[i] = prev_a;
        }
    }
}
