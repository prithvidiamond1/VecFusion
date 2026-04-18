void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    float t0, t1, t2, t3;
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        // Unroll by 4, but note: loop-carried dependency through c[] means
        // each iteration depends on previous c[i-1], so we must be careful.
        // We process sequentially but unroll to help pipelining.
        int limit = LEN_1D - 3;
        for (; i < limit; i += 4) {
            // i+0
            t0 = a[i] + b[i];
            a[i] = t0 + c[i-1];
            t0 = c[i] * d[i];
            c[i] = t0;

            // i+1
            t1 = a[i+1] + b[i+1];
            a[i+1] = t1 + c[i];
            t1 = c[i+1] * d[i+1];
            c[i+1] = t1;

            // i+2
            t2 = a[i+2] + b[i+2];
            a[i+2] = t2 + c[i+1];
            t2 = c[i+2] * d[i+2];
            c[i+2] = t2;

            // i+3
            t3 = a[i+3] + b[i+3];
            a[i+3] = t3 + c[i+2];
            t3 = c[i+3] * d[i+3];
            c[i+3] = t3;
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            float t = a[i] + b[i];
            a[i] = t + c[i-1];
            t = c[i] * d[i];
            c[i] = t;
        }
    }
}
