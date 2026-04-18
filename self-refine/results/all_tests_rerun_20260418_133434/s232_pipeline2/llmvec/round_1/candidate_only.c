#include <stdint.h>

void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    int limit = 100 * (iterations / 256);

    for (int nl = 0; nl < limit; nl++) {
        for (int j = 1; j < 256; j++) {
            float prev = aa[j][0];
            int trip = j; // i goes from 1 to j inclusive

            // The inner loop has a loop-carried dependency on prev,
            // so we cannot vectorize across i. Unroll by 4 manually
            // but the dependency chain means each step must complete first.
            // We keep scalar recurrence but unroll to help instruction scheduling.
            int i = 1;

            // Scalar loop with manual unroll x4 (dependency prevents true SIMD)
            for (; i <= trip - 3; i += 4) {
                float t0 = prev * prev + bb[j][i];
                float t1 = t0 * t0 + bb[j][i+1];
                float t2 = t1 * t1 + bb[j][i+2];
                float t3 = t2 * t2 + bb[j][i+3];
                aa[j][i]   = t0;
                aa[j][i+1] = t1;
                aa[j][i+2] = t2;
                aa[j][i+3] = t3;
                prev = t3;
            }

            // Scalar cleanup tail
            for (; i <= trip; i++) {
                float temp = prev * prev + bb[j][i];
                prev = temp;
                aa[j][i] = temp;
            }
        }
    }
}
