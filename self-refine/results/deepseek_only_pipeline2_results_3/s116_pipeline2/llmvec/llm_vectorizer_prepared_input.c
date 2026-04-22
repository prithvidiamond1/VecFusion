void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations*10; nl++) {
        // Process in forward order with temporary storage
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            // Load all original values before any updates
            float orig_i = a[i];
            float orig_i1 = a[i + 1];
            float orig_i2 = a[i + 2];
            float orig_i3 = a[i + 3];
            float orig_i4 = a[i + 4];
            float orig_i5 = a[i + 5];
            
            // Compute all results using original values
            a[i] = orig_i1 * orig_i;
            a[i + 1] = orig_i2 * orig_i1;
            a[i + 2] = orig_i3 * orig_i2;
            a[i + 3] = orig_i4 * orig_i3;
            a[i + 4] = orig_i5 * orig_i4;
        }
    }
}