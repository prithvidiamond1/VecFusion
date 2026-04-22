float s312_opt(int iterations, int LEN_1D, float* a)
{
    float prod;
    int total_outer = 10 * iterations;
    
    for (int nl = 0; nl < total_outer; nl++) {
        prod = 1.0f;
        
        // Loop distribution: separate reduction from potential other operations
        // Here we only have reduction, so we keep it as is but ensure
        // the reduction variable is properly initialized each outer iteration
        
        // The inner loop has loop-carried dependency (prod depends on previous iteration)
        // This prevents vectorization of the reduction itself
        // However, we can ensure the loop structure is clean for potential
        // compiler optimizations like unrolling
        
        for (int i = 0; i < LEN_1D; i++) {
            prod *= a[i];
        }
    }
    
    return prod;
}
