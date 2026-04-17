#include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        int half = LEN_1D / 2;
        /* Process indices before LEN_1D/2 using the original a[half] value */
        float sv_before = a[half];
        int i = 0;
        /* Unrolled loop for i < half */
        int limit_before = half - (half % 8);
        for (; i < limit_before; i += 8) {
            a[i+0] = sv_before + b[i+0];
            a[i+1] = sv_before + b[i+1];
            a[i+2] = sv_before + b[i+2];
            a[i+3] = sv_before + b[i+3];
            a[i+4] = sv_before + b[i+4];
            a[i+5] = sv_before + b[i+5];
            a[i+6] = sv_before + b[i+6];
            a[i+7] = sv_before + b[i+7];
        }
        /* Scalar tail up to half */
        for (; i < half; i++) {
            a[i] = sv_before + b[i];
        }
        /* Write index half: a[half] = a[half] + b[half], updating a[half] */
        a[half] = sv_before + b[half];
        /* Now a[half] has been updated; use new value for i > half */
        float sv_after = a[half];
        i = half + 1;
        int limit_after = LEN_1D - ((LEN_1D - i) % 8);
        /* Align start for unrolling */
        int unroll_start = i;
        int unroll_limit = LEN_1D - ((LEN_1D - unroll_start) % 8);
        for (; i < unroll_limit; i += 8) {
            a[i+0] = sv_after + b[i+0];
            a[i+1] = sv_after + b[i+1];
            a[i+2] = sv_after + b[i+2];
            a[i+3] = sv_after + b[i+3];
            a[i+4] = sv_after + b[i+4];
            a[i+5] = sv_after + b[i+5];
            a[i+6] = sv_after + b[i+6];
            a[i+7] = sv_after + b[i+7];
        }
        for (; i < LEN_1D; i++) {
            a[i] = sv_after + b[i];
        }
        (void)limit_before;
        (void)limit_after;
    }
}
