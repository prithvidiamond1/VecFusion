#include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

void vectorized_s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        // First loop: for j=1..255, for i=1..255: aa[j][i] = aa[j-1][i] + cc[j][i]
        for (int j = 1; j < 256; j++) {
            int i = 1;
            // Process 4 floats at a time
            for (; i <= 252; i += 4) {
                float4 a_prev, c_cur;
                __builtin_memcpy(&a_prev, &aa[j-1][i], 16);
                __builtin_memcpy(&c_cur,  &cc[j][i],   16);
                float4 result = a_prev + c_cur;
                __builtin_memcpy(&aa[j][i], &result, 16);
            }
            // Scalar tail
            for (; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }

        // Second loop: for i=1..255, for j=1..255: bb[i][j] = bb[i-1][j] + cc[i][j]
        for (int i = 1; i < 256; i++) {
            int j = 1;
            // Process 4 floats at a time
            for (; j <= 252; j += 4) {
                float4 b_prev, c_cur;
                __builtin_memcpy(&b_prev, &bb[i-1][j], 16);
                __builtin_memcpy(&c_cur,  &cc[i][j],   16);
                float4 result = b_prev + c_cur;
                __builtin_memcpy(&bb[i][j], &result, 16);
            }
            // Scalar tail
            for (; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
    }
}
