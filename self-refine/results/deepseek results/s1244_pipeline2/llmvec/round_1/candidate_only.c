#include <stddef.h>

void vectorized_s1244(double * restrict a, double * restrict b, double * restrict c, double * restrict d, int n) {
    typedef double v4d __attribute__((vector_size(32)));

    int i = 0;
    int vec_len = 4;

    for (; i <= n - vec_len; i += vec_len) {
        v4d va0, va1, vres;

        va0 = (v4d){ a[i], a[i+1], a[i+2], a[i+3] };
        va1 = (v4d){ a[i+1], a[i+2], a[i+3], a[i+4] };
        vres = va0 + va1;

        d[i]   = vres[0];
        d[i+1] = vres[1];
        d[i+2] = vres[2];
        d[i+3] = vres[3];
    }

    for (; i < n; i++) {
        d[i] = a[i] + a[i+1];
    }
}
