#include <stddef.h>

float vectorized_s314(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        x = a[0];

        // Process 8 elements at a time using unrolled scalar (vector-friendly)
        int i = 0;
        float x0 = a[0], x1 = a[0], x2 = a[0], x3 = a[0];
        float x4 = a[0], x5 = a[0], x6 = a[0], x7 = a[0];

        for (; i <= LEN_1D - 8; i += 8) {
            if (a[i+0] > x0) x0 = a[i+0];
            if (a[i+1] > x1) x1 = a[i+1];
            if (a[i+2] > x2) x2 = a[i+2];
            if (a[i+3] > x3) x3 = a[i+3];
            if (a[i+4] > x4) x4 = a[i+4];
            if (a[i+5] > x5) x5 = a[i+5];
            if (a[i+6] > x6) x6 = a[i+6];
            if (a[i+7] > x7) x7 = a[i+7];
        }

        // Reduce the 8 accumulators
        if (x1 > x0) x0 = x1;
        if (x2 > x0) x0 = x2;
        if (x3 > x0) x0 = x3;
        if (x4 > x0) x0 = x4;
        if (x5 > x0) x0 = x5;
        if (x6 > x0) x0 = x6;
        if (x7 > x0) x0 = x7;
        x = x0;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            if (a[i] > x) x = a[i];
        }
    }
    return x;
}
