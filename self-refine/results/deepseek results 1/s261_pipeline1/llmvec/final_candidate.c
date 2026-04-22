void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    float t;
    for (int nl = 0; nl < iterations; nl++) {
        // The loop has a carried dependency: c[i] is updated and used as c[i-1] next iteration
        // Process sequentially but unroll by 4 with scalar cleanup
        int i = 1;
        // Scalar unroll by 4
        for (; i <= LEN_1D - 4; i += 4) {
            // iteration i
            t = a[i] + b[i];
            a[i] = t + c[i-1];
            t = c[i] * d[i];
            c[i] = t;

            // iteration i+1
            t = a[i+1] + b[i+1];
            a[i+1] = t + c[i];
            t = c[i+1] * d[i+1];
            c[i+1] = t;

            // iteration i+2
            t = a[i+2] + b[i+2];
            a[i+2] = t + c[i+1];
            t = c[i+2] * d[i+2];
            c[i+2] = t;

            // iteration i+3
            t = a[i+3] + b[i+3];
            a[i+3] = t + c[i+2];
            t = c[i+3] * d[i+3];
            c[i+3] = t;
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            t = a[i] + b[i];
            a[i] = t + c[i-1];
            t = c[i] * d[i];
            c[i] = t;
        }
    }
}
