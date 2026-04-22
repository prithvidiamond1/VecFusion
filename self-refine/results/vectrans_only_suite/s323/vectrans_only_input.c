
void s323_opt(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        float b_prev = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            float c_val = c[i];
            float a_val = b_prev + c_val * d[i];
            a[i] = a_val;
            b_prev = a_val + c_val * e[i];
            b[i] = b_prev;
        }
    }
}
