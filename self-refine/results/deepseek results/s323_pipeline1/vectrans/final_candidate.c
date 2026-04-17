void s323_opt(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        float b_prev = b[0];
        float c_val, d_val, e_val;
        for (int i = 1; i < LEN_1D; i++) {
            c_val = c[i];
            d_val = d[i];
            e_val = e[i];
            a[i] = b_prev + c_val * d_val;
            b_prev = a[i] + c_val * e_val;
            b[i] = b_prev;
        }
    }
}
