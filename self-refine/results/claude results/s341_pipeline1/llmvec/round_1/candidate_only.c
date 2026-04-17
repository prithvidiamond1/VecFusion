void vectorized_s341(int iterations, int LEN_1D, float* a, float *b)
{
    int j;
    for (int nl = 0; nl < iterations; nl++) {
        j = -1;
        // Unroll by 4 for better throughput, but keep scalar logic for correctness
        // since this is a compaction (stream compaction) loop
        int i = 0;
        // Process 4 elements at a time with manual unrolling
        for (; i <= LEN_1D - 4; i += 4) {
            float b0 = b[i+0];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];
            int c0 = b0 > 0.0f;
            int c1 = b1 > 0.0f;
            int c2 = b2 > 0.0f;
            int c3 = b3 > 0.0f;
            if (c0) { j++; a[j] = b0; }
            if (c1) { j++; a[j] = b1; }
            if (c2) { j++; a[j] = b2; }
            if (c3) { j++; a[j] = b3; }
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) {
                j++;
                a[j] = b[i];
            }
        }
    }
}
