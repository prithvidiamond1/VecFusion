#include <stdint.h>

void vectorized_s1113(float* a, float* b, float* out, int n)
{
    int iterations = n;
    int LEN_1D = n;
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float tmp = a[mid];

        // Vectorize first loop: a[i] = tmp + b[i] for i in [0, mid)
        int i = 0;
        int mid4 = mid - (mid % 4);
        for (; i < mid4; i += 4) {
            a[i + 0] = tmp + b[i + 0];
            a[i + 1] = tmp + b[i + 1];
            a[i + 2] = tmp + b[i + 2];
            a[i + 3] = tmp + b[i + 3];
        }
        for (; i < mid; i++) {
            a[i] = tmp + b[i];
        }

        // Middle element
        a[mid] = tmp + b[mid];
        float tmp2 = a[mid];

        // Vectorize second loop: a[i] = tmp2 + b[i] for i in [mid+1, LEN_1D)
        int start = mid + 1;
        int len2 = LEN_1D - start;
        int len2_4 = len2 - (len2 % 4);
        i = start;
        int end4 = start + len2_4;
        for (; i < end4; i += 4) {
            a[i + 0] = tmp2 + b[i + 0];
            a[i + 1] = tmp2 + b[i + 1];
            a[i + 2] = tmp2 + b[i + 2];
            a[i + 3] = tmp2 + b[i + 3];
        }
        for (; i < LEN_1D; i++) {
            a[i] = tmp2 + b[i];
        }
    }

    // Copy result to out if provided
    if (out != (void*)0 && out != a) {
        for (int i = 0; i < LEN_1D; i++) {
            out[i] = a[i];
        }
    }
}
