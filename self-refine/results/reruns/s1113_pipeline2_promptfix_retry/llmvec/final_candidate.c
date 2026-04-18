#include <string.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float mid_val = a[mid];

        // Vectorize first loop: i = 0 to mid-1
        int i = 0;
        for (; i <= mid - 4; i += 4) {
            a[i + 0] = mid_val + b[i + 0];
            a[i + 1] = mid_val + b[i + 1];
            a[i + 2] = mid_val + b[i + 2];
            a[i + 3] = mid_val + b[i + 3];
        }
        for (; i < mid; i++) {
            a[i] = mid_val + b[i];
        }

        // Handle mid element
        a[mid] = mid_val + b[mid];
        mid_val = a[mid];

        // Vectorize second loop: i = mid+1 to LEN_1D-1
        i = mid + 1;
        for (; i <= LEN_1D - 4; i += 4) {
            a[i + 0] = mid_val + b[i + 0];
            a[i + 1] = mid_val + b[i + 1];
            a[i + 2] = mid_val + b[i + 2];
            a[i + 3] = mid_val + b[i + 3];
        }
        for (; i < LEN_1D; i++) {
            a[i] = mid_val + b[i];
        }
    }
}
