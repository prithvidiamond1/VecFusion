void vectorized_s323(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // The loop has a loop-carried dependency through a[i] and b[i]:
        // a[i] = b[i-1] + c[i] * d[i]
        // b[i] = a[i]   + c[i] * e[i]
        // b[i-1] is used in computing a[i], so there's a true dependency.
        // We can unroll manually but cannot fully vectorize across i due to the dependency.
        // However, we can unroll by a factor of 4 to help the compiler pipeline better.

        int i = 1;
        int limit = LEN_1D - 3;

        for (; i < limit; i += 4) {
            float cd0 = c[i+0] * d[i+0];
            float ce0 = c[i+0] * e[i+0];
            float a0 = b[i-1] + cd0;
            float b0 = a0 + ce0;

            float cd1 = c[i+1] * d[i+1];
            float ce1 = c[i+1] * e[i+1];
            float a1 = b0 + cd1;
            float b1 = a1 + ce1;

            float cd2 = c[i+2] * d[i+2];
            float ce2 = c[i+2] * e[i+2];
            float a2 = b1 + cd2;
            float b2 = a2 + ce2;

            float cd3 = c[i+3] * d[i+3];
            float ce3 = c[i+3] * e[i+3];
            float a3 = b2 + cd3;
            float b3 = a3 + ce3;

            a[i+0] = a0; b[i+0] = b0;
            a[i+1] = a1; b[i+1] = b1;
            a[i+2] = a2; b[i+2] = b2;
            a[i+3] = a3; b[i+3] = b3;
        }

        for (; i < LEN_1D; i++) {
            a[i] = b[i-1] + c[i] * d[i];
            b[i] = a[i]   + c[i] * e[i];
        }
    }
}
