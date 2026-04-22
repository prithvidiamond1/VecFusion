void vectorized_s292(int iterations, int LEN_1D, float* a, float* b)
{
    int im1, im2;
    for (int nl = 0; nl < iterations; nl++) {
        im1 = LEN_1D - 1;
        im2 = LEN_1D - 2;
        // The loop has a recurrence on im1/im2 that depends on i,
        // but the values of im1 and im2 for each iteration can be
        // precomputed: for iteration i, im1 = i-1 (mod LEN_1D) and
        // im2 = i-2 (mod LEN_1D).
        // So a[i] = (b[i] + b[(i-1+LEN_1D)%LEN_1D] + b[(i-2+LEN_1D)%LEN_1D]) * 0.333f
        // We can unroll this with direct index computation.

        // Handle first two elements with wrap-around
        // i=0: im1=LEN_1D-1, im2=LEN_1D-2
        // i=1: im1=0, im2=LEN_1D-1
        // i>=2: im1=i-1, im2=i-2

        if (LEN_1D >= 1) {
            a[0] = (b[0] + b[LEN_1D - 1] + b[LEN_1D - 2]) * 0.333f;
        }
        if (LEN_1D >= 2) {
            a[1] = (b[1] + b[0] + b[LEN_1D - 1]) * 0.333f;
        }

        // For i >= 2, no wrap-around needed
        int i = 2;
        // Vectorized portion (unroll by 8)
        int limit = LEN_1D - (LEN_1D - 2) % 8;
        for (; i <= LEN_1D - 8; i += 8) {
            a[i + 0] = (b[i + 0] + b[i - 1] + b[i - 2]) * 0.333f;
            a[i + 1] = (b[i + 1] + b[i + 0] + b[i - 1]) * 0.333f;
            a[i + 2] = (b[i + 2] + b[i + 1] + b[i + 0]) * 0.333f;
            a[i + 3] = (b[i + 3] + b[i + 2] + b[i + 1]) * 0.333f;
            a[i + 4] = (b[i + 4] + b[i + 3] + b[i + 2]) * 0.333f;
            a[i + 5] = (b[i + 5] + b[i + 4] + b[i + 3]) * 0.333f;
            a[i + 6] = (b[i + 6] + b[i + 5] + b[i + 4]) * 0.333f;
            a[i + 7] = (b[i + 7] + b[i + 6] + b[i + 5]) * 0.333f;
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i - 1] + b[i - 2]) * 0.333f;
        }
    }
}
