void s323_opt(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store initial b[0] for first iteration
        float b_prev = b[0];
        
        // Process all i in one pass, maintaining the dependency chain
        for (int i = 1; i < LEN_1D; i++) {
            // Compute a[i] using b[i-1] from current iteration's chain
            float temp_a = b_prev + c[i] * d[i];
            
            // Compute b[i] using newly computed a[i]
            float temp_b = temp_a + c[i] * e[i];
            
            // Store results
            a[i] = temp_a;
            b[i] = temp_b;
            
            // Update b_prev for next iteration
            b_prev = temp_b;
        }
    }
}
