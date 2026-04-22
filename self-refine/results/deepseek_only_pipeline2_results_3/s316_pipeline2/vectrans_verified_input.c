float s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float min_val = a[0];
        int i;
        // Main vectorizable reduction loop
        for (i = 1; i < LEN_1D; ++i) {
            float current = a[i];
            if (current < min_val) {
                min_val = current;
            }
        }
        x = min_val;
    }
    return x;
}