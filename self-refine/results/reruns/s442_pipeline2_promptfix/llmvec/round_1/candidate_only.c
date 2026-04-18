#include <stdint.h>

void vectorized_s442(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx)
{
    int nl, i;
    int iter = iterations / 2;

    int idx1[LEN_1D], idx2[LEN_1D], idx3[LEN_1D], idx4[LEN_1D];
    int count1 = 0, count2 = 0, count3 = 0, count4 = 0;

    for (i = 0; i < LEN_1D; i++) {
        switch (indx[i]) {
            case 1: idx1[count1++] = i; break;
            case 2: idx2[count2++] = i; break;
            case 3: idx3[count3++] = i; break;
            case 4: idx4[count4++] = i; break;
        }
    }

    for (nl = 0; nl < iter; nl++) {
        // Case 1: a[i] += b[i] * b[i]
        {
            int j = 0;
            int limit1 = count1 - (count1 % 4);
            for (; j < limit1; j += 4) {
                int i0 = idx1[j+0];
                int i1 = idx1[j+1];
                int i2 = idx1[j+2];
                int i3 = idx1[j+3];
                a[i0] += b[i0] * b[i0];
                a[i1] += b[i1] * b[i1];
                a[i2] += b[i2] * b[i2];
                a[i3] += b[i3] * b[i3];
            }
            for (; j < count1; j++) {
                int ii = idx1[j];
                a[ii] += b[ii] * b[ii];
            }
        }

        // Case 2: a[i] += c[i] * c[i]
        {
            int j = 0;
            int limit2 = count2 - (count2 % 4);
            for (; j < limit2; j += 4) {
                int i0 = idx2[j+0];
                int i1 = idx2[j+1];
                int i2 = idx2[j+2];
                int i3 = idx2[j+3];
                a[i0] += c[i0] * c[i0];
                a[i1] += c[i1] * c[i1];
                a[i2] += c[i2] * c[i2];
                a[i3] += c[i3] * c[i3];
            }
            for (; j < count2; j++) {
                int ii = idx2[j];
                a[ii] += c[ii] * c[ii];
            }
        }

        // Case 3: a[i] += d[i] * d[i]
        {
            int j = 0;
            int limit3 = count3 - (count3 % 4);
            for (; j < limit3; j += 4) {
                int i0 = idx3[j+0];
                int i1 = idx3[j+1];
                int i2 = idx3[j+2];
                int i3 = idx3[j+3];
                a[i0] += d[i0] * d[i0];
                a[i1] += d[i1] * d[i1];
                a[i2] += d[i2] * d[i2];
                a[i3] += d[i3] * d[i3];
            }
            for (; j < count3; j++) {
                int ii = idx3[j];
                a[ii] += d[ii] * d[ii];
            }
        }

        // Case 4: a[i] += e[i] * e[i]
        {
            int j = 0;
            int limit4 = count4 - (count4 % 4);
            for (; j < limit4; j += 4) {
                int i0 = idx4[j+0];
                int i1 = idx4[j+1];
                int i2 = idx4[j+2];
                int i3 = idx4[j+3];
                a[i0] += e[i0] * e[i0];
                a[i1] += e[i1] * e[i1];
                a[i2] += e[i2] * e[i2];
                a[i3] += e[i3] * e[i3];
            }
            for (; j < count4; j++) {
                int ii = idx4[j];
                a[ii] += e[ii] * e[ii];
            }
        }
    }
}
