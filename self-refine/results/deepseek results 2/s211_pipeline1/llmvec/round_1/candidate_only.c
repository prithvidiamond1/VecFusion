#include <stdint.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Note: b[i] depends on b[i+1] and b[i-1] is used for a[i],
        // but b[i-1] for a[i] uses the OLD b[i-1] before it was updated.
        // The loop updates b[i] = b[i+1] - e[i]*d[i], and a[i] = b[i-1] + c[i]*d[i]
        // where b[i-1] in a[i] refers to the ALREADY UPDATED b[i-1] from previous iteration step.
        // This is a loop-carried dependency on b[], so we must be careful.
        // 
        // Specifically:
        //   a[i] = b[i-1] + c[i]*d[i]   <- uses b[i-1] which was written at step i-1
        //   b[i] = b[i+1] - e[i]*d[i]   <- reads b[i+1] (not yet written), writes b[i]
        //
        // b[i] reads b[i+1] (forward reference, not yet modified), so b updates are safe to vectorize
        // a[i] reads b[i-1] which IS the updated b[i-1] from previous i step.
        // So a[i] depends on updated b[i-1]. We cannot fully vectorize both together.
        //
        // Strategy: compute a[] and b[] in two separate passes per outer iteration.
        // Pass 1: compute all a[i] using current b[] values (b[i-1] before any update)
        // Pass 2: compute all b[i] using current b[] values (b[i+1] not yet overwritten going forward)
        //
        // Wait - in the scalar loop, a[i] uses the UPDATED b[i-1] (from step i-1).
        // To preserve semantics exactly, we cannot split into two passes naively.
        // We must keep the scalar loop order for correctness.
        //
        // However, we can unroll the scalar loop to improve ILP.

        int i;
        int limit = LEN_1D - 1;
        // Unroll by 4, but since there's a loop-carried dep on b[], scalar unrolling only
        for (i = 1; i + 3 < limit; i += 4) {
            float b_im1_0 = b[i - 1];
            a[i] = b_im1_0 + c[i] * d[i];
            b[i] = b[i + 1] - e[i] * d[i];

            float b_im1_1 = b[i];  // updated b[i] used as b[(i+1)-1]
            a[i+1] = b_im1_1 + c[i+1] * d[i+1];
            b[i+1] = b[i + 2] - e[i+1] * d[i+1];

            float b_im1_2 = b[i+1];
            a[i+2] = b_im1_2 + c[i+2] * d[i+2];
            b[i+2] = b[i + 3] - e[i+2] * d[i+2];

            float b_im1_3 = b[i+2];
            a[i+3] = b_im1_3 + c[i+3] * d[i+3];
            b[i+3] = b[i + 4] - e[i+3] * d[i+3];
        }
        // scalar tail
        for (; i < limit; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
            b[i] = b[i + 1] - e[i] * d[i];
        }
    }
}
