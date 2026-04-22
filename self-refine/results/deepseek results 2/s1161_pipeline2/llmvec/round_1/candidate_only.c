#include <string.h>

void vectorized_s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        int n = LEN_1D - 1;
        int i = 0;

        // Vectorized first loop: a[i] = (c[i] >= 0) ? c[i] + d[i]*e[i] : a[i]
        for (; i <= n - 8; i += 8) {
            float c0 = c[i+0], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];
            float c4 = c[i+4], c5 = c[i+5], c6 = c[i+6], c7 = c[i+7];

            float val0 = c0 + d[i+0] * e[i+0];
            float val1 = c1 + d[i+1] * e[i+1];
            float val2 = c2 + d[i+2] * e[i+2];
            float val3 = c3 + d[i+3] * e[i+3];
            float val4 = c4 + d[i+4] * e[i+4];
            float val5 = c5 + d[i+5] * e[i+5];
            float val6 = c6 + d[i+6] * e[i+6];
            float val7 = c7 + d[i+7] * e[i+7];

            if (c0 >= 0.0f) a[i+0] = val0;
            if (c1 >= 0.0f) a[i+1] = val1;
            if (c2 >= 0.0f) a[i+2] = val2;
            if (c3 >= 0.0f) a[i+3] = val3;
            if (c4 >= 0.0f) a[i+4] = val4;
            if (c5 >= 0.0f) a[i+5] = val5;
            if (c6 >= 0.0f) a[i+6] = val6;
            if (c7 >= 0.0f) a[i+7] = val7;
        }
        for (; i < n; i++) {
            if (c[i] >= 0.0f) {
                a[i] = c[i] + d[i] * e[i];
            }
        }

        // Vectorized second loop: b[i] = (c[i] < 0) ? a[i] + d[i]*d[i] : b[i]
        i = 0;
        for (; i <= n - 8; i += 8) {
            float c0 = c[i+0], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];
            float c4 = c[i+4], c5 = c[i+5], c6 = c[i+6], c7 = c[i+7];

            float val0 = a[i+0] + d[i+0] * d[i+0];
            float val1 = a[i+1] + d[i+1] * d[i+1];
            float val2 = a[i+2] + d[i+2] * d[i+2];
            float val3 = a[i+3] + d[i+3] * d[i+3];
            float val4 = a[i+4] + d[i+4] * d[i+4];
            float val5 = a[i+5] + d[i+5] * d[i+5];
            float val6 = a[i+6] + d[i+6] * d[i+6];
            float val7 = a[i+7] + d[i+7] * d[i+7];

            if (c0 < 0.0f) b[i+0] = val0;
            if (c1 < 0.0f) b[i+1] = val1;
            if (c2 < 0.0f) b[i+2] = val2;
            if (c3 < 0.0f) b[i+3] = val3;
            if (c4 < 0.0f) b[i+4] = val4;
            if (c5 < 0.0f) b[i+5] = val5;
            if (c6 < 0.0f) b[i+6] = val6;
            if (c7 < 0.0f) b[i+7] = val7;
        }
        for (; i < n; i++) {
            if (c[i] < 0.0f) {
                b[i] = a[i] + d[i] * d[i];
            }
        }
    }
}
