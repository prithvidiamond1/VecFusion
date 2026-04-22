float s352_opt(int iterations,int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;
        int i = 0;
        
        // Main vectorizable loop - independent partial sums
        for (; i < LEN_1D; i += 5) {
            float sum0 = a[i] * b[i];
            float sum1 = a[i + 1] * b[i + 1];
            float sum2 = a[i + 2] * b[i + 2];
            float sum3 = a[i + 3] * b[i + 3];
            float sum4 = a[i + 4] * b[i + 4];
            
            // Preserve original addition order exactly
            dot = ((dot + sum0) + sum1) + sum2;
            dot = (dot + sum3) + sum4;
        }
    }
    return dot;
}
