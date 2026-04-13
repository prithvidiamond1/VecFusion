#include <stdint.h>

void vectorized_s1113(float* a, float* b, float* out, int n)
{
    int mid = n / 2;
    int iterations = 1;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float tmp = a[mid];

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

        a[mid] = tmp + b[mid];
        float tmp2 = a[mid];

        int start = mid + 1;
        int len2 = n - start;
        int len2_4 = len2 - (len2 % 4);
        i = start;
        int end4 = start + len2_4;
        for (; i < end4; i += 4) {
            a[i + 0] = tmp2 + b[i + 0];
            a[i + 1] = tmp2 + b[i + 1];
            a[i + 2] = tmp2 + b[i + 2];
            a[i + 3] = tmp2 + b[i + 3];
        }
        for (; i < n; i++) {
            a[i] = tmp2 + b[i];
        }
    }

    for (int i = 0; i < n; i++) {
        out[i] = a[i];
    }
}
