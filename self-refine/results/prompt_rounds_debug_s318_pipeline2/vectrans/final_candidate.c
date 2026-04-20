float s318_opt(int iterations,int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;
        
        // Loop distribution: separate absolute value computation from max/index search
        float current_max = max;
        int current_index = 0;
        
        // Vector-friendly loop without branching
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            int update = (abs_val > current_max);
            current_max = update ? abs_val : current_max;
            current_index = update ? i : current_index;
            k += inc;
        }
        
        index = current_index;
        max = current_max;
        
        chksum = max + (float) index;
    }
    return max + index + 1;
}
