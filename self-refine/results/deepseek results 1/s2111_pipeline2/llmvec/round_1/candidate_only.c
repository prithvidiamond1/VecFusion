void vectorized_s2111(float aa[256][256]) {
    for (int j = 1; j < 256; j++) {
        // The inner loop has a loop-carried dependency on aa[j][i-1],
        // so we cannot fully vectorize across i. We unroll by 4 to
        // help the compiler pipeline and reduce loop overhead.
        int i = 1;
        for (; i <= 252; i += 4) {
            float v0 = (aa[j][i-1]   + aa[j-1][i])   / 1.9f;
            aa[j][i] = v0;

            float v1 = (v0            + aa[j-1][i+1]) / 1.9f;
            aa[j][i+1] = v1;

            float v2 = (v1            + aa[j-1][i+2]) / 1.9f;
            aa[j][i+2] = v2;

            float v3 = (v2            + aa[j-1][i+3]) / 1.9f;
            aa[j][i+3] = v3;
        }
        // Scalar cleanup tail
        for (; i < 256; i++) {
            aa[j][i] = (aa[j][i-1] + aa[j-1][i]) / 1.9f;
        }
    }
}
