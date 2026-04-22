float s352_opt(int iterations, int LEN_1D, float* a, float *b)
{
    float dot = (float)0.;
    for (int i = 0; i < LEN_1D; i += 5) {
        dot = dot + a[i] * b[i] + a[i + 1] * b[i + 1] + a[i + 2]
            * b[i + 2] + a[i + 3] * b[i + 3] + a[i + 4] * b[i + 4];
    }
    return dot;
}
