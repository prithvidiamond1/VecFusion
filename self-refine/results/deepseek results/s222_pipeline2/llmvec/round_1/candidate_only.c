void vectorized_s222(int iterations, int LEN_1D, float*a, float*b, float*c, float* e)
{
    if (iterations/2 > 0) {
        float e_prev = e[0];
        for (int i = 1; i < LEN_1D; i++) {
            float e_tmp = e_prev * e_prev;
            e_prev = e_tmp;
            e[i] = e_tmp;
        }
    }
}
