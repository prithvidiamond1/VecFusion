void vectorized_s292(int iterations, int LEN_1D, float* a, float* b)
{
    int im1, im2;
    for (int nl = 0; nl < iterations; nl++) {
        im1 = LEN_1D - 1;
        im2 = LEN_1D - 2;
        // The first two iterations have special index patterns (wrap-around),
        // handle them scalar before vectorizing the rest.
        // i=0: im1=LEN_1D-1, im2=LEN_1D-2
        // i=1: im1=0,         im2=LEN_1D-1
        // i>=2: im1=i-1,      im2=i-2  => fully regular

        // Scalar prologue for i=0 and i=1
        if (LEN_1D > 0) {
            a[0] = (b[0] + b[LEN_1D - 1] + b[LEN_1D - 2]) * 0.333f;
        }
        if (LEN_1D > 1) {
            a[1] = (b[1] + b[0] + b[LEN_1D - 1]) * 0.333f;
        }

        // Vectorized loop for i >= 2: a[i] = (b[i] + b[i-1] + b[i-2]) * 0.333f
        int i = 2;
        int limit = LEN_1D - ((LEN_1D - 2) % 4);

        for (; i < limit; i += 4) {
            a[i]     = (b[i]     + b[i - 1] + b[i - 2]) * 0.333f;
            a[i + 1] = (b[i + 1] + b[i]     + b[i - 1]) * 0.333f;
            a[i + 2] = (b[i + 2] + b[i + 1] + b[i]    ) * 0.333f;
            a[i + 3] = (b[i + 3] + b[i + 2] + b[i + 1]) * 0.333f;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i - 1] + b[i - 2]) * 0.333f;
        }
    }
}
