
float s316_opt(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float min_val = a[0];
        int i;
        // Process in blocks of 4 with independent accumulators
        float min0 = a[0];
        float min1 = a[0];
        float min2 = a[0];
        float min3 = a[0];
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            float val0 = a[i];
            float val1 = a[i+1];
            float val2 = a[i+2];
            float val3 = a[i+3];
            
            // Independent min operations - no loop-carried dependency
            min0 = val0 < min0 ? val0 : min0;
            min1 = val1 < min1 ? val1 : min1;
            min2 = val2 < min2 ? val2 : min2;
            min3 = val3 < min3 ? val3 : min3;
        }
        // Reduce independent accumulators
        float temp_min = min0 < min1 ? min0 : min1;
        float temp_min2 = min2 < min3 ? min2 : min3;
        min_val = temp_min < temp_min2 ? temp_min : temp_min2;
        
        // Handle remaining elements
        for (; i < LEN_1D; i++) {
            float val = a[i];
            min_val = val < min_val ? val : min_val;
        }
        x = min_val;
    }
    return x;
}
