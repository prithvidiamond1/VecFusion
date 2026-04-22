void s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d) {
    float s_sum = s1 + s2;
    
    for (int nl = 0; nl < iterations/5; nl++) {
        float prev = a[0];
        
        // Compute independent parts and store in a temporary array
        float* temp = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        for (int i = 1; i < LEN_1D; ++i) {
            temp[i] = s_sum + b[i] + c[i] + d[i];
        }
        
        // Perform recurrence accumulation
        for (int i = 1; i < LEN_1D; ++i) {
            prev = prev + temp[i];
            a[i] = prev;
        }
    }
}
