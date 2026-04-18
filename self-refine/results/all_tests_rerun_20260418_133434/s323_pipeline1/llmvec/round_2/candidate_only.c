#include <stdint.h>

void vectorized_s323(int iterations, int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        int limit = LEN_1D - 3;

        for (; i < limit; i += 4) {
            float cd0 = c[i]   * d[i];
            float ce0 = c[i]   * e[i];
            a[i]   = b[i-1] + cd0;
            b[i]   = a[i]   + ce0;

            float cd1 = c[i+1] * d[i+1];
            float ce1 = c[i+1] * e[i+1];
            a[i+1] = b[i]   + cd1;
            b[i+1] = a[i+1] + ce1;

            float cd2 = c[i+2] * d[i+2];
            float ce2 = c[i+2] * e[i+2];
            a[i+2] = b[i+1] + cd2;
            b[i+2] = a[i+2] + ce2;

            float cd3 = c[i+3] * d[i+3];
            float ce3 = c[i+3] * e[i+3];
            a[i+3] = b[i+2] + cd3;
            b[i+3] = a[i+3] + ce3;
        }

        for (; i < LEN_1D; i++) {
            a[i] = b[i-1] + c[i] * d[i];
            b[i] = a[i]   + c[i] * e[i];
        }
    }
}
