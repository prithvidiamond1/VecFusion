void vectorized_s342(int iterations, int LEN_1D, float* a, float *b)
{
    // This loop has a data-dependent index j that depends on runtime values of a[i],
    // making true SIMD vectorization of the inner loop semantically complex.
    // We unroll the scalar loop manually for better instruction-level parallelism
    // while preserving exact semantics.
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        int i = 0;

        // Unroll by 4 - but we must be careful: j increments conditionally,
        // so we can't fully vectorize without knowing the branch outcomes.
        // Instead, we unroll to help the compiler with pipelining.
        for (; i <= LEN_1D - 4; i += 4) {
            if (a[i] > 0.0f) {
                j++;
                a[i] = b[j];
            }
            if (a[i+1] > 0.0f) {
                j++;
                a[i+1] = b[j];
            }
            if (a[i+2] > 0.0f) {
                j++;
                a[i+2] = b[j];
            }
            if (a[i+3] > 0.0f) {
                j++;
                a[i+3] = b[j];
            }
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            if (a[i] > 0.0f) {
                j++;
                a[i] = b[j];
            }
        }
    }
}
