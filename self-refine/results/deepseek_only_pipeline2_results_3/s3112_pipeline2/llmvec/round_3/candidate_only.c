float vectorized_s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        float prefix_sum = 0.0f;
        
        // Sequential prefix sum due to dependency
        for (int i = 0; i < LEN_1D; i++) {
            prefix_sum += a[i];
            b[i] = prefix_sum;
        }
        
        sum = prefix_sum;  // Final prefix sum value
    }

    return sum;
}
