#include <stddef.h>

void vectorized_s2111(int iterations, float aa[128]) {
    int total_iters = 100 * (iterations / 128);
    if (total_iters <= 0) return;

    for (int nl = 0; nl < total_iters; nl++) {
        // Process first row (j=1) separately since it only depends on previous column
        for (int i = 1; i < 128; i++) {
            aa[1 * 128 + i] = (aa[1 * 128 + i - 1] + aa[0 * 128 + i]) / 1.9f;
        }

        // Process remaining rows with vectorization-friendly approach
        for (int j = 2; j < 128; j++) {
            int i = 1;
            // Process 4 elements at a time
            for (; i <= 124; i += 4) {
                float t0 = (aa[j * 128 + i - 1] + aa[(j - 1) * 128 + i]) / 1.9f;
                float t1 = (aa[j * 128 + i] + aa[(j - 1) * 128 + i + 1]) / 1.9f;
                float t2 = (aa[j * 128 + i + 1] + aa[(j - 1) * 128 + i + 2]) / 1.9f;
                float t3 = (aa[j * 128 + i + 2] + aa[(j - 1) * 128 + i + 3]) / 1.9f;
                aa[j * 128 + i] = t0;
                aa[j * 128 + i + 1] = t1;
                aa[j * 128 + i + 2] = t2;
                aa[j * 128 + i + 3] = t3;
            }
            // Cleanup for remaining elements
            for (; i < 128; i++) {
                aa[j * 128 + i] = (aa[j * 128 + i - 1] + aa[(j - 1) * 128 + i]) / 1.9f;
            }
        }
    }
}
