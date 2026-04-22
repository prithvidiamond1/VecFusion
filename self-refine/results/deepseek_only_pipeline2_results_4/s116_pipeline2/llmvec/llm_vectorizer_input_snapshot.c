void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            // First, load all needed values into temporaries
            float t0 = a[i];
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];
            
            // Then compute all results
            float r0 = t1 * t0;
            float r1 = t2 * t1;
            float r2 = t3 * t2;
            float r3 = t4 * t3;
            float r4 = t5 * t4;
            
            // Finally, store all results
            a[i] = r0;
            a[i + 1] = r1;
            a[i + 2] = r2;
            a[i + 3] = r3;
            a[i + 4] = r4;
        }
    }
}