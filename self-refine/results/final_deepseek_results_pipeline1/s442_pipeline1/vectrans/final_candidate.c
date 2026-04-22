void s442_opt(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            float square = 0.0f;
            float mask1 = (indx[i] == 1) ? 1.0f : 0.0f;
            float mask2 = (indx[i] == 2) ? 1.0f : 0.0f;
            float mask3 = (indx[i] == 3) ? 1.0f : 0.0f;
            float mask4 = (indx[i] == 4) ? 1.0f : 0.0f;
            
            square += mask1 * b[i] * b[i];
            square += mask2 * c[i] * c[i];
            square += mask3 * d[i] * d[i];
            square += mask4 * e[i] * e[i];
            
            a[i] += square;
        }
    }
}
