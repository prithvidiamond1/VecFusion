void vectorized_s322(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    // This loop has a recurrence (a[i] depends on a[i-1] and a[i-2]),
    // so true vectorization of the inner loop is not straightforward.
    // We provide an unrolled scalar version that is vectorization-friendly
    // in structure but preserves correct semantics.
    for (int nl = 0; nl < iterations/2; nl++) {
        float a_i_minus_2 = a[0];
        float a_i_minus_1 = a[1];

        int i = 2;
        // Unroll by 4 - but due to recurrence, each iteration depends on previous,
        // so we must compute sequentially. Unrolling helps the compiler pipeline.
        for (; i <= LEN_1D - 4; i += 4) {
            float a0 = a[i];
            float r0 = a0 + a_i_minus_1 * b[i] + a_i_minus_2 * c[i];
            a[i] = r0;

            float a1 = a[i+1];
            float r1 = a1 + r0 * b[i+1] + a_i_minus_1 * c[i+1];
            a[i+1] = r1;

            float a2 = a[i+2];
            float r2 = a2 + r1 * b[i+2] + r0 * c[i+2];
            a[i+2] = r2;

            float a3 = a[i+3];
            float r3 = a3 + r2 * b[i+3] + r1 * c[i+3];
            a[i+3] = r3;

            a_i_minus_2 = r2;
            a_i_minus_1 = r3;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float a_i = a[i];
            float result = a_i + a_i_minus_1 * b[i] + a_i_minus_2 * c[i];
            a_i_minus_2 = a_i_minus_1;
            a_i_minus_1 = result;
            a[i] = result;
        }
    }
}
