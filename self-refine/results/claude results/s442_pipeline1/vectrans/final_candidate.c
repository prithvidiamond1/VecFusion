void s442_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            int idx = indx[i];
            float val;
            if (idx == 2)
                val = c[i] * c[i];
            else if (idx == 3)
                val = d[i] * d[i];
            else if (idx == 4)
                val = e[i] * e[i];
            else
                val = b[i] * b[i];
            a[i] += val;
        }
    }
}
