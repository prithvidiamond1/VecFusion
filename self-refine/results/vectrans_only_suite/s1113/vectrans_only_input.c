
void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float a_mid_old = a[mid];
        
        for (int i = 0; i < mid; i++) {
            a[i] = a_mid_old + b[i];
        }
        
        float a_mid_new = a_mid_old + b[mid];
        a[mid] = a_mid_new;
        
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = a_mid_new + b[i];
        }
    }
}
