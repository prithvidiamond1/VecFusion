float s316(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        float temp_x = x;
        for (int i = 1; i < LEN_1D; ++i) {
            float current = a[i];
            if (current < temp_x) {
                temp_x = current;
            }
        }
        x = temp_x;
    }
    return x;
}
