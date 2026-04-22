
void s2251_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        
        // Precompute all s_next values into a temporary array
        // This breaks the loop-carried dependency on s
        float* s_next_vals = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        
        // First compute all s_next values (b[i]+c[i])
        // This loop has no loop-carried dependencies and can be vectorized
        for (int i = 0; i < LEN_1D; i++) {
            s_next_vals[i] = b[i] + c[i];
        }
        
        // Now compute a[i] and b[i] using precomputed s values
        // Process first iteration with initial s=0
        a[0] = s * e[0];
        b[0] = a[0] + d[0];
        s = s_next_vals[0];
        
        // Main loop: a[i] uses s from previous iteration, which is now s_next_vals[i-1]
        // This loop can be vectorized since s_next_vals[i-1] is already computed
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = s * e[i];
            b[i] = a[i] + d[i];
            s = s_next_vals[i];
        }
    }
}
