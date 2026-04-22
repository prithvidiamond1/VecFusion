#include <stdint.h>

void vectorized_s453(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations * 2; nl++) {
        // s_acc at index i = 2.0 * (i+1)
        // a[i] = s_acc * b[i] = 2.0f * (i+1) * b[i]

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);

        for (; i < limit; i += 8) {
            float s0 = 2.0f * (float)(i + 1);
            float s1 = 2.0f * (float)(i + 2);
            float s2 = 2.0f * (float)(i + 3);
            float s3 = 2.0f * (float)(i + 4);
            float s4 = 2.0f * (float)(i + 5);
            float s5 = 2.0f * (float)(i + 6);
            float s6 = 2.0f * (float)(i + 7);
            float s7 = 2.0f * (float)(i + 8);

            a[i + 0] = s0 * b[i + 0];
            a[i + 1] = s1 * b[i + 1];
            a[i + 2] = s2 * b[i + 2];
            a[i + 3] = s3 * b[i + 3];
            a[i + 4] = s4 * b[i + 4];
            a[i + 5] = s5 * b[i + 5];
            a[i + 6] = s6 * b[i + 6];
            a[i + 7] = s7 * b[i + 7];
        }

        for (; i < LEN_1D; i++) {
            float s_acc = 2.0f * (float)(i + 1);
            a[i] = s_acc * b[i];
        }
    }
}
