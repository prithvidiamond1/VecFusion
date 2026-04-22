void s453(int iterations,int LEN_1D, float* a, float *b)
{
    float s;
    for (int nl = 0; nl < iterations*2; nl++) {
        s = 0.;
        // Loop distribution: separate the accumulation from the multiplication
        // First, compute all s values in a vectorizable way
        float s_acc = 0.;
        for (int i = 0; i < LEN_1D; i++) {
            s_acc += (float)2.;
            // Store accumulated value in a temporary array
            float temp_s = s_acc;
            a[i] = temp_s * b[i];
        }
    }
}