void s442(int iterations,int LEN_1D, float* a, float *b,float* c, float *d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Process each index sequentially as in original, but group by condition
        for (int i = 0; i < LEN_1D; i++) {
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
        }
    }
}