void s1113(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Split the loop into three parts to handle the dependency on a[mid]
        // Part 1: i < mid (uses original a[mid] value)
        float mid_val = a[mid];
        for (int i = 0; i < mid; i++) {
            a[i] = mid_val + b[i];
        }
        
        // Part 2: i == mid (update a[mid] using its original value)
        a[mid] = mid_val + b[mid];
        
        // Part 3: i > mid (uses the updated a[mid] value)
        float updated_mid_val = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = updated_mid_val + b[i];
        }
    }
}