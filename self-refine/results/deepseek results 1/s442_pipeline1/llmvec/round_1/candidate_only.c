void vectorized_s442(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            int idx = indx[i];
            float val;
            switch (idx) {
                case 1:
                    val = b[i] * b[i];
                    break;
                case 2:
                    val = c[i] * c[i];
                    break;
                case 3:
                    val = d[i] * d[i];
                    break;
                case 4:
                    val = e[i] * e[i];
                    break;
                default:
                    /* Original code falls through to L15 for default case */
                    val = b[i] * b[i];
                    break;
            }
            a[i] += val;
        }
    }
}
