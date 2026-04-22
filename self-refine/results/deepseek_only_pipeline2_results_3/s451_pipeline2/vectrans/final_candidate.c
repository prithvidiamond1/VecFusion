void s451_opt(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int loop_count = iterations / 5;
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Fused loop with temporary storage to enable vectorization
        // while maintaining single memory write per element
        for (int i = 0; i < LEN_1D; i++) {
            float temp = sinf(b[i]) + cosf(c[i]);
            a[i] = temp;
        }
    }
}
