void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float a_mid_original = a[mid];
        float a_mid_updated = a_mid_original + b[mid];
        
        for (int i = 0; i < mid; i++) {
            a[i] = a_mid_original + b[i];
        }
        
        a[mid] = a_mid_updated;
        
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = a_mid_updated + b[i];
        }
    }
}
