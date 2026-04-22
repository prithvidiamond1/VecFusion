float s319_opt(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e)
{
    float sum = 0.f;
    
    // First loop: compute a[i] and accumulate sum
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = c[i] + d[i];
        sum += a[i];
    }
    
    // Second loop: compute b[i] and accumulate sum
    for (int i = 0; i < LEN_1D; i++) {
        b[i] = c[i] + e[i];
        sum += b[i];
    }
    
    return sum;
}
