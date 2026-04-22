void vectorized_s291(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // First element: im1 = LEN_1D-1
        a[0] = (b[0] + b[LEN_1D - 1]) * 0.5f;

        // Remaining elements: a[i] = (b[i] + b[i-1]) * 0.5
        // This is a simple shift-and-add, vectorizable since reads are from b (not a)
        int i = 1;

        // Vectorized portion using unrolling (groups of 8)
        for (; i <= LEN_1D - 8; i += 8) {
            a[i + 0] = (b[i + 0] + b[i - 1]) * 0.5f;
            a[i + 1] = (b[i + 1] + b[i + 0]) * 0.5f;
            a[i + 2] = (b[i + 2] + b[i + 1]) * 0.5f;
            a[i + 3] = (b[i + 3] + b[i + 2]) * 0.5f;
            a[i + 4] = (b[i + 4] + b[i + 3]) * 0.5f;
            a[i + 5] = (b[i + 5] + b[i + 4]) * 0.5f;
            a[i + 6] = (b[i + 6] + b[i + 5]) * 0.5f;
            a[i + 7] = (b[i + 7] + b[i + 6]) * 0.5f;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i - 1]) * 0.5f;
        }
    }
}
