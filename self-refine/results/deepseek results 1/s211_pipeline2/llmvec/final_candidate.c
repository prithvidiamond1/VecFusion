#include <stddef.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        int n = LEN_1D - 2; // number of elements from index 1 to LEN_1D-2

        // Step 1: Compute b[i] = b[i+1] - e[i]*d[i] for i in [1, LEN_1D-2)
        // We need to read b[i+1] before writing b[i], so process in a vectorized pass
        // using a temporary buffer stored on the stack or via VLA.
        // We'll use a VLA for the temp buffer.
        float b_plus1[LEN_1D];

        // Vectorized store of b[i+1] into b_plus1[i]
        {
            int i = 1;
            // Process 8 at a time
            for (; i <= n - 8; i += 8) {
                b_plus1[i+0] = b[i+1];
                b_plus1[i+1] = b[i+2];
                b_plus1[i+2] = b[i+3];
                b_plus1[i+3] = b[i+4];
                b_plus1[i+4] = b[i+5];
                b_plus1[i+5] = b[i+6];
                b_plus1[i+6] = b[i+7];
                b_plus1[i+7] = b[i+8];
            }
            for (; i < LEN_1D - 1; i++) {
                b_plus1[i] = b[i+1];
            }
        }

        // Vectorized compute b[i] = b_plus1[i] - e[i]*d[i]
        {
            int i = 1;
            for (; i <= n - 8; i += 8) {
                b[i+0] = b_plus1[i+0] - e[i+0] * d[i+0];
                b[i+1] = b_plus1[i+1] - e[i+1] * d[i+1];
                b[i+2] = b_plus1[i+2] - e[i+2] * d[i+2];
                b[i+3] = b_plus1[i+3] - e[i+3] * d[i+3];
                b[i+4] = b_plus1[i+4] - e[i+4] * d[i+4];
                b[i+5] = b_plus1[i+5] - e[i+5] * d[i+5];
                b[i+6] = b_plus1[i+6] - e[i+6] * d[i+6];
                b[i+7] = b_plus1[i+7] - e[i+7] * d[i+7];
            }
            for (; i < LEN_1D - 1; i++) {
                b[i] = b_plus1[i] - e[i] * d[i];
            }
        }

        // Vectorized compute a[i] = b[i-1] + c[i]*d[i]
        {
            int i = 1;
            for (; i <= n - 8; i += 8) {
                a[i+0] = b[i-1] + c[i+0] * d[i+0];
                a[i+1] = b[i+0] + c[i+1] * d[i+1];
                a[i+2] = b[i+1] + c[i+2] * d[i+2];
                a[i+3] = b[i+2] + c[i+3] * d[i+3];
                a[i+4] = b[i+3] + c[i+4] * d[i+4];
                a[i+5] = b[i+4] + c[i+5] * d[i+5];
                a[i+6] = b[i+5] + c[i+6] * d[i+6];
                a[i+7] = b[i+6] + c[i+7] * d[i+7];
            }
            for (; i < LEN_1D - 1; i++) {
                a[i] = b[i-1] + c[i] * d[i];
            }
        }
    }
}
