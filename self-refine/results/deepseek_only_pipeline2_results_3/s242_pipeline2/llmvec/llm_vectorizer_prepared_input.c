void s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    float sum_s = s1 + s2;
    float prev;
    
    for (int nl = 0; nl < iterations/5; nl++) {
        // Store initial value for dependency chain
        prev = a[0];
        
        // Vectorizable loop: compute independent sums
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = sum_s + b[i] + c[i] + d[i];
        }
        
        // Sequential recurrence: maintain dependency
        for (int i = 1; i < LEN_1D; ++i) {
            prev = prev + a[i];
            a[i] = prev;
        }
    }
}