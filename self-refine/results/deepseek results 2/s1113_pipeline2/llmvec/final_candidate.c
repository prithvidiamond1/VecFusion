#include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;

    for (int nl = 0; nl < 2 * iterations; nl++) {
        float a_mid_value = a[mid];

        // Vectorize the first loop: i in [0, mid)
        int i = 0;
        int mid4 = (mid / 4) * 4;
        for (; i < mid4; i += 4) {
            a[i+0] = a_mid_value + b[i+0];
            a[i+1] = a_mid_value + b[i+1];
            a[i+2] = a_mid_value + b[i+2];
            a[i+3] = a_mid_value + b[i+3];
        }
        for (; i < mid; i++) {
            a[i] = a_mid_value + b[i];
        }

        // Update mid
        a[mid] = a_mid_value + b[mid];

        // Vectorize the second loop: i in [mid+1, LEN_1D)
        float updated_mid_value = a[mid];
        int start = mid + 1;
        int end = LEN_1D;
        int len2 = end - start;
        int len2_4 = (len2 / 4) * 4;
        i = start;
        int end4 = start + len2_4;
        for (; i < end4; i += 4) {
            a[i+0] = updated_mid_value + b[i+0];
            a[i+1] = updated_mid_value + b[i+1];
            a[i+2] = updated_mid_value + b[i+2];
            a[i+3] = updated_mid_value + b[i+3];
        }
        for (; i < end; i++) {
            a[i] = updated_mid_value + b[i];
        }
    }
}
