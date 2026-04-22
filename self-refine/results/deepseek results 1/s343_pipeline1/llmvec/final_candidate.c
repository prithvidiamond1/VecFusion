#include <string.h>

void vectorized_s343(int iterations, float * flat_2d_array, float aa[256][256], float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = -1;
        for (int i = 0; i < 256; i++) {
            // Process j in chunks of 8 for vectorization
            int j = 0;
            for (; j + 7 < 256; j += 8) {
                float b0 = bb[j+0][i];
                float b1 = bb[j+1][i];
                float b2 = bb[j+2][i];
                float b3 = bb[j+3][i];
                float b4 = bb[j+4][i];
                float b5 = bb[j+5][i];
                float b6 = bb[j+6][i];
                float b7 = bb[j+7][i];

                float a0 = aa[j+0][i];
                float a1 = aa[j+1][i];
                float a2 = aa[j+2][i];
                float a3 = aa[j+3][i];
                float a4 = aa[j+4][i];
                float a5 = aa[j+5][i];
                float a6 = aa[j+6][i];
                float a7 = aa[j+7][i];

                int m0 = b0 > 0.0f;
                int m1 = b1 > 0.0f;
                int m2 = b2 > 0.0f;
                int m3 = b3 > 0.0f;
                int m4 = b4 > 0.0f;
                int m5 = b5 > 0.0f;
                int m6 = b6 > 0.0f;
                int m7 = b7 > 0.0f;

                if (m0) { k++; flat_2d_array[k] = a0; }
                if (m1) { k++; flat_2d_array[k] = a1; }
                if (m2) { k++; flat_2d_array[k] = a2; }
                if (m3) { k++; flat_2d_array[k] = a3; }
                if (m4) { k++; flat_2d_array[k] = a4; }
                if (m5) { k++; flat_2d_array[k] = a5; }
                if (m6) { k++; flat_2d_array[k] = a6; }
                if (m7) { k++; flat_2d_array[k] = a7; }
            }
            // Scalar cleanup
            for (; j < 256; j++) {
                if (bb[j][i] > 0.0f) {
                    k++;
                    flat_2d_array[k] = aa[j][i];
                }
            }
        }
    }
}
