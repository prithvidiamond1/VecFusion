void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Process symmetric pairs together to break the dependency
        int i = 0;
        int j = LEN_1D - 1;
        
        // Process pairs from both ends towards the middle
        for (; i < j; i++, j--) {
            // Process element i (reads from original a[j])
            float x_i = a[j] + b[i] * c[i];
            a[i] = x_i - 1.0f;
            b[i] = x_i;
            
            // Process element j (reads from original a[i] which hasn't been updated yet)
            float x_j = a[i] + b[j] * c[j];
            a[j] = x_j - 1.0f;
            b[j] = x_j;
        }
        
        // Handle middle element when LEN_1D is odd
        if (i == j) {
            float x = a[i] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }
    }
}