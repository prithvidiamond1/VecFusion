
float s3113_opt(int iterations,int LEN_1D, float* a)
{
    float max;
    int outer_loop_count = iterations * 4;
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        float current_max = fabsf(a[0]);
        
        for (int i = 0; i < LEN_1D; i++) {
            float abs_val = fabsf(a[i]);
            if (abs_val > current_max) {
                current_max = abs_val;
            }
        }
        
        max = current_max;
    }
    
    return max;
}
