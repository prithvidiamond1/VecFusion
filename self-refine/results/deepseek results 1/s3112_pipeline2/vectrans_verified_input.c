float s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;
        float temp_sum = (float)0.0;
        
        // Prefix sum loop (sequential dependency remains)
        for (int i = 0; i < LEN_1D; i++) {
            temp_sum += a[i];
            b[i] = temp_sum;
        }
        
        // Use the final temp_sum as sum (eliminates redundant second loop)
        sum = temp_sum;
    }
    return sum;
}