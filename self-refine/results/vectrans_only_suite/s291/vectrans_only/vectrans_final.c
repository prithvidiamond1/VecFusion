
void s291_opt(int iterations,int LEN_1D, float* a,float* b) {
    for (int nl = 0; nl < 2*iterations; nl++) {
        float last_b = b[LEN_1D-1];
        for (int i = 0; i < LEN_1D; i++) {
            float current_b = b[i];
            a[i] = (current_b + last_b) * 0.5f;
            last_b = current_b;
        }
    }
}
