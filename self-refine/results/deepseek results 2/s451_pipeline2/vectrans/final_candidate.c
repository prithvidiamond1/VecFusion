void s451_opt(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int loop_count = iterations / 5;
    
    for (int nl = 0; nl < loop_count; nl++) {
        // No loop-carried dependencies - each iteration writes to a[i] 
        // using only b[i] and c[i], which are read-only in this loop
        // The operations are independent across i, so we can keep the loop structure
        // as is for vectorization, but we can split the computation if needed
        
        // Since sinf(b[i]) and cosf(c[i]) are independent computations,
        // we could split them, but they both feed into the same assignment.
        // Instead, we keep the simple structure which is already vectorizable
        
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = sinf(b[i]) + cosf(c[i]);
        }
    }
}
