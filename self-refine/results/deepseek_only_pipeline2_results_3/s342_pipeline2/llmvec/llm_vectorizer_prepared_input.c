void s342(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute condition mask and store indices
        int count = 0;
        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > (float)0.) {
                count++;
            }
        }
        
        // Second pass: update positive elements using sequential j
        int j = -1;
        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > (float)0.) {
                j++;
                a[i] = b[j];
            }
        }
    }
}