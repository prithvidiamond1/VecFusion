#include <stddef.h>

void vectorized_s323(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int loop_count = iterations / 2;

    for (int nl = 0; nl < loop_count; nl++) {
        // Prefix sum for b - scalar sequential (not vectorizable due to recurrence)
        float b_prev = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b_prev + c[i] * (d[i] + e[i]);
            b_prev = b[i];
        }

        // Compute a[1] separately
        a[1] = b[0] + c[1] * d[1];

        // Vectorized computation for a[i] = b[i-1] + c[i] * d[i]
        int i = 2;
        // Process groups of 4 where i <= LEN_1D-4
        for (; i <= LEN_1D - 4; i += 4) {
            // Load b[i-1] values for indices i, i+1, i+2, i+3
            float b_prev_0 = b[i-1];
            float b_prev_1 = b[i];
            float b_prev_2 = b[i+1];
            float b_prev_3 = b[i+2];

            // Compute c[i] * d[i] for four consecutive elements
            float prod_0 = c[i] * d[i];
            float prod_1 = c[i+1] * d[i+1];
            float prod_2 = c[i+2] * d[i+2];
            float prod_3 = c[i+3] * d[i+3];

            // Store results
            a[i] = b_prev_0 + prod_0;
            a[i+1] = b_prev_1 + prod_1;
            a[i+2] = b_prev_2 + prod_2;
            a[i+3] = b_prev_3 + prod_3;
        }

        // Scalar tail for remaining a elements (including case when LEN_1D < 3)
        for (; i < LEN_1D; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}
