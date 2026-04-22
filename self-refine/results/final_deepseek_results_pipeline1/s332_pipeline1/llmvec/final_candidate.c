#include <stdint.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a) {
    float value;
    int index;
    float chksum;
    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;
        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > (float)t) {
                index = i;
                value = a[i];
                break;
            }
        }
        chksum = value + (float)index;
    }
    return value;
}
