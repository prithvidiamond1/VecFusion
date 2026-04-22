float s314(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            float current = a[i];
            if (current > x) {
                x = current;
            }
        }
    }
    return x;
}