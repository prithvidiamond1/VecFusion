#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float* b)
{
    float dot;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        dot = 0.0f;

        // Each iteration of the original loop processes 5 elements at a time.
        // We unroll by processing 4 groups of 5 (= 20 elements) per iteration
        // to expose more parallelism.
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 20); // multiple of 20

        float acc0 = 0.0f, acc1 = 0.0f, acc2 = 0.0f, acc3 = 0.0f;

        for (; i < limit; i += 20) {
            // Group 0: i+0..i+4
            acc0 += a[i + 0] * b[i + 0]
                  + a[i + 1] * b[i + 1]
                  + a[i + 2] * b[i + 2]
                  + a[i + 3] * b[i + 3]
                  + a[i + 4] * b[i + 4];
            // Group 1: i+5..i+9
            acc1 += a[i + 5] * b[i + 5]
                  + a[i + 6] * b[i + 6]
                  + a[i + 7] * b[i + 7]
                  + a[i + 8] * b[i + 8]
                  + a[i + 9] * b[i + 9];
            // Group 2: i+10..i+14
            acc2 += a[i + 10] * b[i + 10]
                  + a[i + 11] * b[i + 11]
                  + a[i + 12] * b[i + 12]
                  + a[i + 13] * b[i + 13]
                  + a[i + 14] * b[i + 14];
            // Group 3: i+15..i+19
            acc3 += a[i + 15] * b[i + 15]
                  + a[i + 16] * b[i + 16]
                  + a[i + 17] * b[i + 17]
                  + a[i + 18] * b[i + 18]
                  + a[i + 19] * b[i + 19];
        }

        dot = acc0 + acc1 + acc2 + acc3;

        // Scalar cleanup for remaining groups of 5
        for (; i < LEN_1D; i += 5) {
            dot += a[i + 0] * b[i + 0]
                 + a[i + 1] * b[i + 1]
                 + a[i + 2] * b[i + 2]
                 + a[i + 3] * b[i + 3]
                 + a[i + 4] * b[i + 4];
        }
    }
    return dot;
}
