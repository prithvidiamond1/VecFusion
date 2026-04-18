void vectorized_s2111(int iterations, float aa[256][256])
{
    int nl_limit = 100 * (iterations / 256);

    for (int nl = 0; nl < nl_limit; nl++) {
        for (int j = 1; j < 256; j++) {
            // The inner loop has a loop-carried dependency on aa[j][i-1],
            // so we cannot vectorize across i. We unroll by 4 to help ILP.
            int i = 1;
            // Unroll by 4, but each iteration still depends on previous
            for (; i <= 252; i += 4) {
                aa[j][i]   = (aa[j][i-1]   + aa[j-1][i])   / 1.9f;
                aa[j][i+1] = (aa[j][i]     + aa[j-1][i+1]) / 1.9f;
                aa[j][i+2] = (aa[j][i+1]   + aa[j-1][i+2]) / 1.9f;
                aa[j][i+3] = (aa[j][i+2]   + aa[j-1][i+3]) / 1.9f;
            }
            // Scalar cleanup
            for (; i < 256; i++) {
                aa[j][i] = (aa[j][i-1] + aa[j-1][i]) / 1.9f;
            }
        }
    }
}
