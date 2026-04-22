void vectorized_s341(int iterations, int LEN_1D, float* a, float *b)
{
    int j;
    for (int nl = 0; nl < iterations; nl++) {
        j = -1;
        // Unroll by 4 for better vectorization hints, but keep scalar logic
        // since this is a compaction (stream compaction) which is hard to
        // fully vectorize without scatter. We unroll to help the compiler.
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
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
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) {
                j++;
                a[j] = b[i];
            }
        }
    }
}
