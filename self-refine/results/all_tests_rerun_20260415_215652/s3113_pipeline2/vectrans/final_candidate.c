float s3113_opt(int iterations, int LEN_1D, float* a)
{
    float max = fabsf(a[0]);
    for (int i = 1; i < LEN_1D; i++) {
        float abs_val = fabsf(a[i]);
        max = abs_val > max ? abs_val : max;
    }
    return max;
}
