void vectorized_s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        int len = LEN_1D - 1;

#if defined(__clang__) || defined(__GNUC__)
        typedef float float4 __attribute__((vector_size(16)));

        int vec_len = len - (len % 4);
        for (; i < vec_len; i += 4) {
            float4 ci, di, ei, ai, bi;

            /* Load */
            ci[0] = c[i+0]; ci[1] = c[i+1]; ci[2] = c[i+2]; ci[3] = c[i+3];
            di[0] = d[i+0]; di[1] = d[i+1]; di[2] = d[i+2]; di[3] = d[i+3];
            ei[0] = e[i+0]; ei[1] = e[i+1]; ei[2] = e[i+2]; ei[3] = e[i+3];
            ai[0] = a[i+0]; ai[1] = a[i+1]; ai[2] = a[i+2]; ai[3] = a[i+3];

            /* Compute both branches */
            float4 a_branch = ci + di * ei;       /* c[i] >= 0 branch */
            float4 b_branch = ai + di * di;       /* c[i] <  0 branch */

            /* mask: c[i] < 0 */
            float4 zero = {0.f, 0.f, 0.f, 0.f};
            /* Use element-wise selection */
            for (int k = 0; k < 4; k++) {
                if (ci[k] < 0.f) {
                    b[i+k] = b_branch[k];
                } else {
                    a[i+k] = a_branch[k];
                }
            }
        }
#endif

        /* Scalar tail (or full loop if no vector extension) */
        for (; i < len; i++) {
            if (c[i] < (float)0.) {
                b[i] = a[i] + d[i] * d[i];
            } else {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}
