#include <stddef.h>

extern float c_original[];

void vectorized_s161(int ntimes, int len, int *ip, float *restrict a,
                     float *restrict b, float *restrict c, float *restrict d,
                     float *restrict e, float aa[][256], float bb[][256],
                     float cc[][256]) {
    typedef float v4f __attribute__((vector_size(16)));
    const int VEC = 4;

    for (int nl = 0; nl < ntimes; nl++) {
        int limit = len - (len % VEC);

        for (int i = 0; i < limit; i += VEC) {
            v4f ai = *(v4f *)(a + i);
            v4f bi = *(v4f *)(b + i);
            v4f ci = *(v4f *)(c + i);
            v4f di = *(v4f *)(d + i);
            v4f ei = *(v4f *)(e + i);

            v4f res = ai + bi * ci + di * ei;
            *(v4f *)(a + i) = res;
        }

        for (int i = limit; i < len; i++) {
            a[i] = a[i] + b[i] * c[i] + d[i] * e[i];
        }
    }
}
