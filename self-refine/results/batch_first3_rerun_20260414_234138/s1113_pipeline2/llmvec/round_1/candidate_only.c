#include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float mid_val = a[mid];

        // Vectorize first loop: i = 0 to mid-1
        int i = 0;
        int mid_aligned = mid - (mid % 8);
        for (; i < mid_aligned; i += 8) {
            a[i+0] = mid_val + b[i+0];
            a[i+1] = mid_val + b[i+1];
            a[i+2] = mid_val + b[i+2];
            a[i+3] = mid_val + b[i+3];
            a[i+4] = mid_val + b[i+4];
            a[i+5] = mid_val + b[i+5];
            a[i+6] = mid_val + b[i+6];
            a[i+7] = mid_val + b[i+7];
        }
        for (; i < mid; i++) {
            a[i] = mid_val + b[i];
        }

        // Handle mid element
        a[mid] = mid_val + b[mid];
        float new_mid_val = a[mid];

        // Vectorize second loop: i = mid+1 to LEN_1D-1
        int len2 = LEN_1D - (mid + 1);
        int start = mid + 1;
        int len2_aligned = len2 - (len2 % 8);
        i = 0;
        for (; i < len2_aligned; i += 8) {
            a[start+i+0] = new_mid_val + b[start+i+0];
            a[start+i+1] = new_mid_val + b[start+i+1];
            a[start+i+2] = new_mid_val + b[start+i+2];
            a[start+i+3] = new_mid_val + b[start+i+3];
            a[start+i+4] = new_mid_val + b[start+i+4];
            a[start+i+5] = new_mid_val + b[start+i+5];
            a[start+i+6] = new_mid_val + b[start+i+6];
            a[start+i+7] = new_mid_val + b[start+i+7];
        }
        for (; i < len2; i++) {
            a[start+i] = new_mid_val + b[start+i];
        }
    }
}
