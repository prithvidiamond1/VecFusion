void vectorized_s442(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        int i = 0;
        /* Process in chunks of 4 for vectorization */
        for (; i <= LEN_1D - 4; i += 4) {
            for (int j = 0; j < 4; j++) {
                int idx = indx[i+j];
                if (idx == 1) {
                    a[i+j] += b[i+j] * b[i+j];
                } else if (idx == 2) {
                    a[i+j] += c[i+j] * c[i+j];
                } else if (idx == 3) {
                    a[i+j] += d[i+j] * d[i+j];
                } else if (idx == 4) {
                    a[i+j] += e[i+j] * e[i+j];
                }
                /* default: no update */
            }
        }
        /* scalar tail */
        for (; i < LEN_1D; i++) {
            int idx = indx[i];
            if (idx == 1) {
                a[i] += b[i] * b[i];
            } else if (idx == 2) {
                a[i] += c[i] * c[i];
            } else if (idx == 3) {
                a[i] += d[i] * d[i];
            } else if (idx == 4) {
                a[i] += e[i] * e[i];
            }
            /* default: no update */
        }
    }
}
