float s3112_opt(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;
        
        // Loop distribution: separate reduction from prefix sum
        // First compute prefix sum into b
        if (LEN_1D > 0) {
            b[0] = a[0];
        }
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b[i-1] + a[i];
        }
        
        // Then compute total sum via vectorizable reduction
        float temp_sum = (float)0.0;
        for (int i = 0; i < LEN_1D; i++) {
            temp_sum += a[i];
        }
        sum = temp_sum;
    }
    return sum;
}
