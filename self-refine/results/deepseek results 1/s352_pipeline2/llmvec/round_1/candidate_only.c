#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;

        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f;
        float dot4 = 0.0f, dot5 = 0.0f, dot6 = 0.0f, dot7 = 0.0f;
        float dot8 = 0.0f, dot9 = 0.0f, dot10 = 0.0f, dot11 = 0.0f;
        float dot12 = 0.0f, dot13 = 0.0f, dot14 = 0.0f, dot15 = 0.0f;
        float dot16 = 0.0f, dot17 = 0.0f, dot18 = 0.0f, dot19 = 0.0f;

        int i;

        // Unrolled by 20 (4 groups of 5) for better vectorization
        for (i = 0; i <= LEN_1D - 20; i += 20) {
            dot0  += a[i+0]  * b[i+0];
            dot1  += a[i+1]  * b[i+1];
            dot2  += a[i+2]  * b[i+2];
            dot3  += a[i+3]  * b[i+3];
            dot4  += a[i+4]  * b[i+4];
            dot5  += a[i+5]  * b[i+5];
            dot6  += a[i+6]  * b[i+6];
            dot7  += a[i+7]  * b[i+7];
            dot8  += a[i+8]  * b[i+8];
            dot9  += a[i+9]  * b[i+9];
            dot10 += a[i+10] * b[i+10];
            dot11 += a[i+11] * b[i+11];
            dot12 += a[i+12] * b[i+12];
            dot13 += a[i+13] * b[i+13];
            dot14 += a[i+14] * b[i+14];
            dot15 += a[i+15] * b[i+15];
            dot16 += a[i+16] * b[i+16];
            dot17 += a[i+17] * b[i+17];
            dot18 += a[i+18] * b[i+18];
            dot19 += a[i+19] * b[i+19];
        }

        // Handle remaining elements in groups of 5
        for (; i <= LEN_1D - 5; i += 5) {
            dot0 += a[i+0] * b[i+0];
            dot1 += a[i+1] * b[i+1];
            dot2 += a[i+2] * b[i+2];
            dot3 += a[i+3] * b[i+3];
            dot4 += a[i+4] * b[i+4];
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }

        // Combine all partial sums
        dot += (dot0 + dot1 + dot2 + dot3)
             + (dot4 + dot5 + dot6 + dot7)
             + (dot8 + dot9 + dot10 + dot11)
             + (dot12 + dot13 + dot14 + dot15)
             + (dot16 + dot17 + dot18 + dot19);
    }
    return dot;
}
