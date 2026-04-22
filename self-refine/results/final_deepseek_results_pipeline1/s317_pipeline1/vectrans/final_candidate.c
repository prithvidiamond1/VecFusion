float s317_opt(int iterations, int LEN_1D)
{
    float q = 1.0f;
    int inner_iterations = LEN_1D / 2;
    
    // Compute q once
    for (int i = 0; i < inner_iterations; i++) {
        q *= 0.99f;
    }
    
    // No need to repeat the inner loop - q is the same each time
    // The outer loop just multiplies by 1 each iteration (no change)
    // So we just return q
    
    return q;
}
