#include <stddef.h>

void vectorized_s323(int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    // This loop has a loop-carried dependency through a[] and b[]:
    // a[i] depends on b[i-1], and b[i] depends on a[i].
    // The dependency chain: b[i-1] -> a[i] -> b[i] -> a[i+1] -> ...
    // This means we cannot fully vectorize across iterations without breaking semantics.
    // However, we can unroll to expose instruction-level parallelism within each iteration.

    // Unroll by 4 but keep sequential dependency chain
    int i = 1;
    int limit = LEN_1D - 4;

    for (; i <= limit; i += 4) {
        float ci0 = c[i];
        float di0 = d[i];
        float ei0 = e[i];
        float a0 = b[i-1] + ci0 * di0;
        float b0 = a0 + ci0 * ei0;

        float ci1 = c[i+1];
        float di1 = d[i+1];
        float ei1 = e[i+1];
        float a1 = b0 + ci1 * di1;
        float b1 = a1 + ci1 * ei1;

        float ci2 = c[i+2];
        float di2 = d[i+2];
        float ei2 = e[i+2];
        float a2 = b1 + ci2 * di2;
        float b2 = a2 + ci2 * ei2;

        float ci3 = c[i+3];
        float di3 = d[i+3];
        float ei3 = e[i+3];
        float a3 = b2 + ci3 * di3;
        float b3 = a3 + ci3 * ei3;

        a[i]   = a0; b[i]   = b0;
        a[i+1] = a1; b[i+1] = b1;
        a[i+2] = a2; b[i+2] = b2;
        a[i+3] = a3; b[i+3] = b3;
    }

    // Scalar cleanup tail
    for (; i < LEN_1D; i++) {
        a[i] = b[i-1] + c[i] * d[i];
        b[i] = a[i] + c[i] * e[i];
    }
}
