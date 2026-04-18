void vectorized_s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // The inner loop has a loop-carried dependency through a[i+1] in the b[i] update,
        // and b[i] is read in the a[i] update but written after. We must preserve scalar semantics.
        // a[i] = b[i] * c[i] * d[i]  (uses old b[i])
        // b[i] = a[i] * a[i+1] * d[i] (uses new a[i] and new a[i+1])
        // Since a[i+1] is computed in the next iteration, we can split into two passes:
        // Pass 1: compute all a[i] from old b[i], c[i], d[i]
        // Pass 2: compute all b[i] from new a[i], a[i+1], d[i]

        int i = 0;
        int limit = LEN_1D - 1;

        // Pass 1: a[i] = b[i] * c[i] * d[i] for i in [0, LEN_1D-2]
        // Vectorize with width 8
        int vec_limit = limit - (limit % 8);
        for (i = 0; i < vec_limit; i += 8) {
            a[i+0] = b[i+0] * c[i+0] * d[i+0];
            a[i+1] = b[i+1] * c[i+1] * d[i+1];
            a[i+2] = b[i+2] * c[i+2] * d[i+2];
            a[i+3] = b[i+3] * c[i+3] * d[i+3];
            a[i+4] = b[i+4] * c[i+4] * d[i+4];
            a[i+5] = b[i+5] * c[i+5] * d[i+5];
            a[i+6] = b[i+6] * c[i+6] * d[i+6];
            a[i+7] = b[i+7] * c[i+7] * d[i+7];
        }
        for (; i < limit; i++) {
            a[i] = b[i] * c[i] * d[i];
        }

        // Pass 2: b[i] = a[i] * a[i+1] * d[i] for i in [0, LEN_1D-2]
        vec_limit = limit - (limit % 8);
        for (i = 0; i < vec_limit; i += 8) {
            b[i+0] = a[i+0] * a[i+1] * d[i+0];
            b[i+1] = a[i+1] * a[i+2] * d[i+1];
            b[i+2] = a[i+2] * a[i+3] * d[i+2];
            b[i+3] = a[i+3] * a[i+4] * d[i+3];
            b[i+4] = a[i+4] * a[i+5] * d[i+4];
            b[i+5] = a[i+5] * a[i+6] * d[i+5];
            b[i+6] = a[i+6] * a[i+7] * d[i+6];
            b[i+7] = a[i+7] * a[i+8] * d[i+7];
        }
        for (; i < limit; i++) {
            b[i] = a[i] * a[i+1] * d[i];
        }
    }
}
