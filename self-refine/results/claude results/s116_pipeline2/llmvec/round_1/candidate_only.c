#include <stdint.h>

void vectorized_s116(int iterations, int LEN_1D, float* a)
{
    for (int nl = 0; nl < iterations * 10; nl++) {
        // The inner loop steps by 5 and uses a[i..i+5].
        // Each iteration: a[i+k] = a[i+k+1] * a[i+k] for k=0..4
        // We unroll by processing 4 groups of 5 at a time (20 elements),
        // reading the values first, then writing.
        int i = 0;
        int limit = LEN_1D - 5;

        // Process 4 groups (stride=5) at a time = 20 elements per unrolled iteration
        for (; i + 20 <= limit; i += 20) {
            // Group 0: i+0..i+5
            float g0_a0 = a[i + 0];
            float g0_a1 = a[i + 1];
            float g0_a2 = a[i + 2];
            float g0_a3 = a[i + 3];
            float g0_a4 = a[i + 4];
            float g0_a5 = a[i + 5];

            // Group 1: i+5..i+10
            float g1_a0 = a[i + 5];
            float g1_a1 = a[i + 6];
            float g1_a2 = a[i + 7];
            float g1_a3 = a[i + 8];
            float g1_a4 = a[i + 9];
            float g1_a5 = a[i + 10];

            // Group 2: i+10..i+15
            float g2_a0 = a[i + 10];
            float g2_a1 = a[i + 11];
            float g2_a2 = a[i + 12];
            float g2_a3 = a[i + 13];
            float g2_a4 = a[i + 14];
            float g2_a5 = a[i + 15];

            // Group 3: i+15..i+20
            float g3_a0 = a[i + 15];
            float g3_a1 = a[i + 16];
            float g3_a2 = a[i + 17];
            float g3_a3 = a[i + 18];
            float g3_a4 = a[i + 19];
            float g3_a5 = a[i + 20];

            // Write group 0
            a[i + 0] = g0_a1 * g0_a0;
            a[i + 1] = g0_a2 * g0_a1;
            a[i + 2] = g0_a3 * g0_a2;
            a[i + 3] = g0_a4 * g0_a3;
            a[i + 4] = g0_a5 * g0_a4;

            // Write group 1
            a[i + 5]  = g1_a1 * g1_a0;
            a[i + 6]  = g1_a2 * g1_a1;
            a[i + 7]  = g1_a3 * g1_a2;
            a[i + 8]  = g1_a4 * g1_a3;
            a[i + 9]  = g1_a5 * g1_a4;

            // Write group 2
            a[i + 10] = g2_a1 * g2_a0;
            a[i + 11] = g2_a2 * g2_a1;
            a[i + 12] = g2_a3 * g2_a2;
            a[i + 13] = g2_a4 * g2_a3;
            a[i + 14] = g2_a5 * g2_a4;

            // Write group 3
            a[i + 15] = g3_a1 * g3_a0;
            a[i + 16] = g3_a2 * g3_a1;
            a[i + 17] = g3_a3 * g3_a2;
            a[i + 18] = g3_a4 * g3_a3;
            a[i + 19] = g3_a5 * g3_a4;
        }

        // Scalar cleanup for remaining groups
        for (; i < limit; i += 5) {
            float a0 = a[i];
            float a1 = a[i + 1];
            float a2 = a[i + 2];
            float a3 = a[i + 3];
            float a4 = a[i + 4];
            float a5 = a[i + 5];
            a[i]     = a1 * a0;
            a[i + 1] = a2 * a1;
            a[i + 2] = a3 * a2;
            a[i + 3] = a4 * a3;
            a[i + 4] = a5 * a4;
        }
    }
}
