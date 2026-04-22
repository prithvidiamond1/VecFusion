void vectorized_s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // This loop has loop-carried dependencies through a[i+1] and b[i],
        // so we cannot vectorize across iterations of i.
        // We unroll manually but keep scalar semantics.
        int i = 0;
        // Process pairs of iterations unrolled
        for (; i < LEN_1D - 2; i += 2) {
            // Iteration i
            float bi0 = b[i];
            float ci0 = c[i];
            float di0 = d[i];
            float ai0 = b[i] + ci0 * di0;
            a[i] = ai0;
            float bi0_new = ci0 + bi0;
            b[i] = bi0_new;
            float ai1_old = a[i+1];
            float ai1_new = bi0_new + ai1_old * di0;
            a[i+1] = ai1_new;

            // Iteration i+1
            float bi1 = b[i+1];
            float ci1 = c[i+1];
            float di1 = d[i+1];
            float ai1_final = bi1 + ci1 * di1;
            a[i+1] = ai1_final;
            float bi1_new = ci1 + bi1;
            b[i+1] = bi1_new;
            float ai2_old = a[i+2];
            float ai2_new = bi1_new + ai2_old * di1;
            a[i+2] = ai2_new;
        }
        // Scalar cleanup
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i] + c[i] * d[i];
            b[i] = c[i] + b[i];
            a[i+1] = b[i] + a[i+1] * d[i];
        }
    }
}
