void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    float* a_original = (float*)malloc(LEN_1D * sizeof(float));
    
    for (int nl = 0; nl < iterations; nl++) {
        // Copy 'a' to temporary array to preserve original values
        for (int i = 0; i < LEN_1D; i++) {
            a_original[i] = a[i];
        }
        
        // Process all iterations using preserved 'a' values
        for (int i = 0; i < LEN_1D; i++) {
            float x = a_original[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }
    
    free(a_original);
}