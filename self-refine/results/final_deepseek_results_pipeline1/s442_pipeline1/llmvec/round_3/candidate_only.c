#include <stdint.h>

void vectorized_s442(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx) {
    if (LEN_1D <= 0) return;
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        for (; i <= LEN_1D - 4; i += 4) {
            int idx0 = indx[i];
            int idx1 = indx[i+1];
            int idx2 = indx[i+2];
            int idx3 = indx[i+3];

            float add0, add1, add2, add3;

            if (idx0 == 1) add0 = b[i] * b[i];
            else if (idx0 == 2) add0 = c[i] * c[i];
            else if (idx0 == 3) add0 = d[i] * d[i];
            else add0 = e[i] * e[i];

            if (idx1 == 1) add1 = b[i+1] * b[i+1];
            else if (idx1 == 2) add1 = c[i+1] * c[i+1];
            else if (idx1 == 3) add1 = d[i+1] * d[i+1];
            else add1 = e[i+1] * e[i+1];

            if (idx2 == 1) add2 = b[i+2] * b[i+2];
            else if (idx2 == 2) add2 = c[i+2] * c[i+2];
            else if (idx2 == 3) add2 = d[i+2] * d[i+2];
            else add2 = e[i+2] * e[i+2];

            if (idx3 == 1) add3 = b[i+3] * b[i+3];
            else if (idx3 == 2) add3 = c[i+3] * c[i+3];
            else if (idx3 == 3) add3 = d[i+3] * d[i+3];
            else add3 = e[i+3] * e[i+3];

            a[i] += add0;
            a[i+1] += add1;
            a[i+2] += add2;
            a[i+3] += add3;
        }
        for (; i < LEN_1D; i++) {
            switch (indx[i]) {
                case 1:  a[i] += b[i] * b[i]; break;
                case 2:  a[i] += c[i] * c[i]; break;
                case 3:  a[i] += d[i] * d[i]; break;
                default: a[i] += e[i] * e[i]; break;
            }
        }
    }
}
