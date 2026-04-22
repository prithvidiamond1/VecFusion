float s352(int iterations,int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;
        
        // Create multiple partial sums to break dependency chain
        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f, dot4 = 0.0f;
        
        // Process groups of 5 elements with 5 independent accumulators
        int i;
        for (i = 0; i + 4 < LEN_1D; i += 5) {
            // Each accumulator handles one element from the group
            dot0 += a[i] * b[i];
            dot1 += a[i + 1] * b[i + 1];
            dot2 += a[i + 2] * b[i + 2];
            dot3 += a[i + 3] * b[i + 3];
            dot4 += a[i + 4] * b[i + 4];
        }
        
        // Combine partial sums
        dot = dot0 + dot1 + dot2 + dot3 + dot4;
        
        // Handle remaining elements (if any)
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}
