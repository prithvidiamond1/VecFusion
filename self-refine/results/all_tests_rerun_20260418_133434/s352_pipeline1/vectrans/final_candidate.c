float s352_opt(int iterations,int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.f;
        int i;
        int limit = LEN_1D - 4;
        float dot0 = 0.f, dot1 = 0.f, dot2 = 0.f, dot3 = 0.f, dot4 = 0.f;
        for (i = 0; i < limit; i += 5) {
            float t0 = a[i] * b[i];
            float t1 = a[i+1] * b[i+1];
            float t2 = a[i+2] * b[i+2];
            float t3 = a[i+3] * b[i+3];
            float t4 = a[i+4] * b[i+4];
            dot0 += t0;
            dot1 += t1;
            dot2 += t2;
            dot3 += t3;
            dot4 += t4;
        }
        dot = dot0 + dot1 + dot2 + dot3 + dot4;
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}
