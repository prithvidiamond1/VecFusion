
void s116_opt(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        // Process in groups of 5 with temporary storage to break dependencies
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            // Store original right-hand side values before any updates
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];
            
            // Now compute all updates using original values
            a[i] = t1 * a[i];
            a[i + 1] = t2 * t1;
            a[i + 2] = t3 * t2;
            a[i + 3] = t4 * t3;
            a[i + 4] = t5 * t4;
        }
    }
}
