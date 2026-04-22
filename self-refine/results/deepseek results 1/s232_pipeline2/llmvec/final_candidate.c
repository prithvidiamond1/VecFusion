void vectorized_s232(int iterations, float aa[256][256], float bb[256][256]) {
    int outer_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        // Process j in groups of 4 for ILP, but each j has different bounds
        // so we handle the triangular recurrence per-j
        int j = 1;
        for (; j <= 252; j += 4) {
            int j0 = j, j1 = j+1, j2 = j+2, j3 = j+3;

            float prev0 = aa[j0][0] * aa[j0][0] + bb[j0][1];
            float prev1 = aa[j1][0] * aa[j1][0] + bb[j1][1];
            float prev2 = aa[j2][0] * aa[j2][0] + bb[j2][1];
            float prev3 = aa[j3][0] * aa[j3][0] + bb[j3][1];

            aa[j0][1] = prev0;
            aa[j1][1] = prev1;
            aa[j2][1] = prev2;
            aa[j3][1] = prev3;

            // j0 loop: i from 2 to j0
            for (int i = 2; i <= j0; i++) {
                prev0 = prev0 * prev0 + bb[j0][i];
                aa[j0][i] = prev0;
            }
            // j1 loop: i from 2 to j1
            // reuse prev0 result for i <= j0, then continue
            // Actually each j is independent, reset prev from aa
            prev1 = aa[j1][1];
            for (int i = 2; i <= j1; i++) {
                prev1 = prev1 * prev1 + bb[j1][i];
                aa[j1][i] = prev1;
            }
            prev2 = aa[j2][1];
            for (int i = 2; i <= j2; i++) {
                prev2 = prev2 * prev2 + bb[j2][i];
                aa[j2][i] = prev2;
            }
            prev3 = aa[j3][1];
            for (int i = 2; i <= j3; i++) {
                prev3 = prev3 * prev3 + bb[j3][i];
                aa[j3][i] = prev3;
            }
        }
        // Scalar cleanup for remaining j values
        for (; j < 256; j++) {
            float prev = aa[j][0] * aa[j][0] + bb[j][1];
            aa[j][1] = prev;
            for (int i = 2; i <= j; i++) {
                prev = prev * prev + bb[j][i];
                aa[j][i] = prev;
            }
        }
    }
}
