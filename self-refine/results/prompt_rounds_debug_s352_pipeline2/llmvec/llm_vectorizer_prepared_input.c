float s352(int iterations,int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;
        int i;
        for (i = 0; i < LEN_1D; i += 5) {
            float t0 = a[i] * b[i];
            float t1 = a[i+1] * b[i+1];
            float t2 = a[i+2] * b[i+2];
            float t3 = a[i+3] * b[i+3];
            float t4 = a[i+4] * b[i+4];
            dot += t0 + t1 + t2 + t3 + t4;
        }
    }
    return dot;
}