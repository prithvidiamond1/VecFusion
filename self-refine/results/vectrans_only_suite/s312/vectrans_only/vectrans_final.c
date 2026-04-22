
float s312_opt(int iterations, int LEN_1D, float* a)
{
    float prod;
    for (int nl = 0; nl < 10*iterations; nl++) {
        prod = (float)1.;
        
        // Loop distribution: break the reduction into independent chunks
        // Use multiple accumulators to break loop-carried dependency
        const int CHUNK_SIZE = 4;
        float accum[4] = {(float)1., (float)1., (float)1., (float)1.};
        int i;
        
        // Process in chunks of 4 - each accumulator processes contiguous elements
        for (i = 0; i + 3 < LEN_1D; i += 4) {
            // Load contiguous elements into temporaries
            float temp0 = a[i];
            float temp1 = a[i+1];
            float temp2 = a[i+2];
            float temp3 = a[i+3];
            
            // Multiply accumulators with their corresponding elements
            accum[0] *= temp0;
            accum[1] *= temp1;
            accum[2] *= temp2;
            accum[3] *= temp3;
        }
        
        // Combine chunk results
        prod = accum[0] * accum[1] * accum[2] * accum[3];
        
        // Handle remaining elements
        for (; i < LEN_1D; i++) {
            float temp = a[i];
            prod *= temp;
        }
    }
    return prod;
}
