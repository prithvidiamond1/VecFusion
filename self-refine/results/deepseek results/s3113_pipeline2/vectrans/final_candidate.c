float s3113_opt(int iterations,int LEN_1D, float* a)
{
    float max;
    for (int nl = 0; nl < iterations*4; nl++) {
        float current_max = fabsf(a[0]);
        int i;
        
        // Process first element separately to simplify loop
        for (i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[i]);
            if (abs_val > current_max) {
                current_max = abs_val;
            }
        }
        
        max = current_max;
    }
    return max;
}
