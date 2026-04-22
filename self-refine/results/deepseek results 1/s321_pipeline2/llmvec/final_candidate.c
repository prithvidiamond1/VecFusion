void vectorized_s321(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        float prev = a[0];

        // This is a sequential recurrence: a[i] = a[i] + a[i-1] * b[i]
        // Each iteration depends on the previous result, so true vectorization
        // is not possible without changing semantics. We unroll manually for
        // better instruction-level parallelism hints, but must remain sequential.

        int i = 1;

        // Unroll by 4 - but since each step depends on prev, we must compute sequentially
        // We can at least help the compiler with explicit unrolling
        for (; i <= LEN_1D - 4; i += 4) {
            float p0 = prev * b[i];
            a[i] += p0;
            prev = a[i];

            float p1 = prev * b[i+1];
            a[i+1] += p1;
            prev = a[i+1];

            float p2 = prev * b[i+2];
            a[i+2] += p2;
            prev = a[i+2];

            float p3 = prev * b[i+3];
            a[i+3] += p3;
            prev = a[i+3];
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float product = prev * b[i];
            a[i] += product;
            prev = a[i];
        }
    }
}
