void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations/5; nl++) {
        // This loop has a loop-carried dependency on a[i] = a[i-1] + ...
        // so we cannot fully vectorize the inner loop without breaking semantics.
        // We unroll manually but keep the dependency chain intact.
        float s12 = s1 + s2;
        int i = 1;
        // Unroll by 4 while respecting the dependency
        for (; i <= LEN_1D - 4; i += 4) {
            a[i]     = a[i - 1] + s12 + b[i]     + c[i]     + d[i];
            a[i + 1] = a[i]     + s12 + b[i + 1] + c[i + 1] + d[i + 1];
            a[i + 2] = a[i + 1] + s12 + b[i + 2] + c[i + 2] + d[i + 2];
            a[i + 3] = a[i + 2] + s12 + b[i + 3] + c[i + 3] + d[i + 3];
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] = a[i - 1] + s12 + b[i] + c[i] + d[i];
        }
    }
}
