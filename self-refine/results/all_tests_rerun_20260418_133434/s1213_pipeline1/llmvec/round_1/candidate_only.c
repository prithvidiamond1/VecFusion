void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Due to loop-carried dependency (b[i] = a[i+1]*d[i] where a[i+1] is
        // written in next iteration), we must preserve sequential order.
        // Unroll by 4 but keep sequential semantics.
        int i = 1;
        int limit = LEN_1D - 1;
        int unroll_limit = limit - ((limit - 1) % 4);

        for (; i < unroll_limit - 3; i += 4) {
            a[i]   = b[i-1]   + c[i];
            b[i]   = a[i+1]   * d[i];
            a[i+1] = b[i]     + c[i+1];
            b[i+1] = a[i+2]   * d[i+1];
            a[i+2] = b[i+1]   + c[i+2];
            b[i+2] = a[i+3]   * d[i+2];
            a[i+3] = b[i+2]   + c[i+3];
            b[i+3] = a[i+4]   * d[i+3];
        }

        // Scalar cleanup
        for (; i < limit; i++) {
            a[i] = b[i-1] + c[i];
            b[i] = a[i+1] * d[i];
        }
    }
}
