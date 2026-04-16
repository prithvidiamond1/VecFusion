void vectorized_s221(int iterations, int LEN_1D, float*a, float*b, float*c, float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // The inner loop has a loop-carried dependency in b[i] = b[i-1] + a[i] + d[i],
        // so b cannot be vectorized. However, a[i] += c[i] * d[i] can be vectorized
        // as a first pass, then b must be computed serially.

        // Pass 1: vectorize a[i] += c[i] * d[i] for i in [1, LEN_1D)
        int i = 1;
        int len = LEN_1D - 1; // number of elements to process

#if defined(__clang__) || defined(__GNUC__)
        typedef float float4 __attribute__((vector_size(16)));
        int vec_len = len & ~3; // round down to multiple of 4
        int vec_end = 1 + vec_len;

        for (; i < vec_end; i += 4) {
            float4 ai, ci, di;
            // Load 4 elements
            __builtin_memcpy(&ai, &a[i], 16);
            __builtin_memcpy(&ci, &c[i], 16);
            __builtin_memcpy(&di, &d[i], 16);
            ai = ai + ci * di;
            __builtin_memcpy(&a[i], &ai, 16);
        }
#endif
        // Scalar tail for a
        for (; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }

        // Pass 2: serial computation of b due to loop-carried dependency
        for (i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
