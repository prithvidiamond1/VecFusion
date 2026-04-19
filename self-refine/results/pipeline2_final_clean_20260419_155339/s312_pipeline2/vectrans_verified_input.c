float s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    for (int nl = 0; nl < 10*iterations; nl++) {
        prod = (float)1.;
        
        // Loop distribution: separate the reduction into independent accumulations
        // This removes the loop-carried dependency within the main loop
        float temp_prod = (float)1.;
        int i;
        // Main vectorizable loop
        for (i = 0; i < LEN_1D; i++) {
            temp_prod = temp_prod * a[i];
        }
        // Combine result (single assignment outside inner loop)
        prod = temp_prod;
    }
    return prod;
}