#include <stdint.h>

void vectorized_s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int total_nl = 200 * (iterations / 256);

    for (int nl = 0; nl < total_nl; nl++) {
        // First: compute all a[i] updates - vectorized with unrolling
        int i = 0;
        for (; i <= 256 - 8; i += 8) {
            a[i+0] += b[i+0] * c[i+0];
            a[i+1] += b[i+1] * c[i+1];
            a[i+2] += b[i+2] * c[i+2];
            a[i+3] += b[i+3] * c[i+3];
            a[i+4] += b[i+4] * c[i+4];
            a[i+5] += b[i+5] * c[i+5];
            a[i+6] += b[i+6] * c[i+6];
            a[i+7] += b[i+7] * c[i+7];
        }
        for (; i < 256; i++) {
            a[i] += b[i] * c[i];
        }

        // Second: compute aa columns independently
        // Process multiple columns at once (unroll over i)
        int col = 0;
        for (; col <= 256 - 4; col += 4) {
            float a_val0 = a[col+0];
            float a_val1 = a[col+1];
            float a_val2 = a[col+2];
            float a_val3 = a[col+3];

            float aa_prev0 = aa[0][col+0];
            float aa_prev1 = aa[0][col+1];
            float aa_prev2 = aa[0][col+2];
            float aa_prev3 = aa[0][col+3];

            for (int j = 1; j < 256; j++) {
                aa_prev0 = aa_prev0 + bb[j][col+0] * a_val0;
                aa_prev1 = aa_prev1 + bb[j][col+1] * a_val1;
                aa_prev2 = aa_prev2 + bb[j][col+2] * a_val2;
                aa_prev3 = aa_prev3 + bb[j][col+3] * a_val3;
                aa[j][col+0] = aa_prev0;
                aa[j][col+1] = aa_prev1;
                aa[j][col+2] = aa_prev2;
                aa[j][col+3] = aa_prev3;
            }
        }
        for (; col < 256; col++) {
            float a_val = a[col];
            float aa_prev = aa[0][col];
            for (int j = 1; j < 256; j++) {
                aa_prev = aa_prev + bb[j][col] * a_val;
                aa[j][col] = aa_prev;
            }
        }
    }
}
