#include <stdint.h>

void vectorized_s222(int iterations, int LEN_1D, float*a, float*b, float*c, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        /* a[i] += b[i]*c[i] then a[i] -= b[i]*c[i] in the same iteration:
           net effect on a[] is zero, so no update needed. */

        /* e[] has a loop-carried dependency: e[i] = e[i-1] * e[i-1] */
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i-1] * e[i-1];
        }
    }
}
