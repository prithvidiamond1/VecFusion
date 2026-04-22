void s442_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            int idx = indx[i];
            int use_c = (idx == 2);
            int use_d = (idx == 3);
            int use_e = (idx == 4);
            int use_b = !use_c & !use_d & !use_e;
            float val = (float)use_b * (b[i] * b[i])
                      + (float)use_c * (c[i] * c[i])
                      + (float)use_d * (d[i] * d[i])
                      + (float)use_e * (e[i] * e[i]);
            a[i] += val;
        }
    }
}
