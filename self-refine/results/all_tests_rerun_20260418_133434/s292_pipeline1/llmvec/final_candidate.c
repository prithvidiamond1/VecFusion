void vectorized_s292(int iterations, int LEN_1D, float* a, float* b)
{
    int im1, im2;
    for (int nl = 0; nl < iterations; nl++) {
        im1 = LEN_1D - 1;
        im2 = LEN_1D - 2;
        // The first two iterations have dependencies on im1/im2 that wrap around,
        // handle them scalar first, then vectorize the rest.
        // Actually, let's just unroll manually in groups of 4 where possible.
        // For i=0: im1=LEN_1D-1, im2=LEN_1D-2
        // For i=1: im1=0,        im2=LEN_1D-1
        // For i=2: im1=1,        im2=0
        // For i=k (k>=2): im1=k-1, im2=k-2
        // So for i>=2, a[i] = (b[i] + b[i-1] + b[i-2]) * 0.333f
        // Handle i=0 and i=1 scalar:
        if (LEN_1D > 0) {
            a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * 0.333f;
        }
        if (LEN_1D > 1) {
            a[1] = (b[1] + b[0] + b[LEN_1D-1]) * 0.333f;
        }
        // Vectorize i=2..LEN_1D-1
        int i = 2;
        int limit = LEN_1D - ((LEN_1D - 2) % 4);
        for (; i < limit; i += 4) {
            a[i]   = (b[i]   + b[i-1] + b[i-2]) * 0.333f;
            a[i+1] = (b[i+1] + b[i]   + b[i-1]) * 0.333f;
            a[i+2] = (b[i+2] + b[i+1] + b[i])   * 0.333f;
            a[i+3] = (b[i+3] + b[i+2] + b[i+1]) * 0.333f;
        }
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * 0.333f;
        }
    }
}
