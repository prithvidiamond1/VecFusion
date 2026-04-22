
void s451_opt(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int loop_count = iterations / 5;
    
    for (int nl = 0; nl < loop_count; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            float sin_val = sinf(b[i]);
            float cos_val = cosf(c[i]);
            a[i] = sin_val + cos_val;
        }
    }
}
