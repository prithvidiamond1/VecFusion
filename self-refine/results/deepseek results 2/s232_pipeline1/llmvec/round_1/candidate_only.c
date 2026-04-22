void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int j = 1; j < 256; j++) {
            // The inner loop has a loop-carried dependency:
            // aa[j][i] = aa[j][i-1]*aa[j][i-1] + bb[j][i]
            // Each iteration depends on the previous, so true vectorization
            // is not possible. We unroll manually for better pipelining.
            int i = 1;
            // Unroll by 4, but since there's a loop-carried dependency,
            // we must compute sequentially. Unroll to help the compiler pipeline.
            int limit = j - 3;
            for (; i <= limit; i += 4) {
                float prev0 = aa[j][i-1];
                aa[j][i]   = prev0 * prev0 + bb[j][i];
                float prev1 = aa[j][i];
                aa[j][i+1] = prev1 * prev1 + bb[j][i+1];
                float prev2 = aa[j][i+1];
                aa[j][i+2] = prev2 * prev2 + bb[j][i+2];
                float prev3 = aa[j][i+2];
                aa[j][i+3] = prev3 * prev3 + bb[j][i+3];
            }
            // Scalar cleanup
            for (; i <= j; i++) {
                aa[j][i] = aa[j][i-1] * aa[j][i-1] + bb[j][i];
            }
        }
    }
}
