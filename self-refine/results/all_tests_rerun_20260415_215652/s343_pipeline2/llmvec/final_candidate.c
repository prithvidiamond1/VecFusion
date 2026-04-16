#include <string.h>

void vectorized_s343(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int cond[256][256];
    int row_count[256];
    int row_start[257];

    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        // Compute conditions with unrolling for vectorization
        for (int j = 0; j < 256; j++) {
            int i = 0;
            for (; i <= 256 - 8; i += 8) {
                cond[j][i+0] = (bb[j][i+0] > 0.0f);
                cond[j][i+1] = (bb[j][i+1] > 0.0f);
                cond[j][i+2] = (bb[j][i+2] > 0.0f);
                cond[j][i+3] = (bb[j][i+3] > 0.0f);
                cond[j][i+4] = (bb[j][i+4] > 0.0f);
                cond[j][i+5] = (bb[j][i+5] > 0.0f);
                cond[j][i+6] = (bb[j][i+6] > 0.0f);
                cond[j][i+7] = (bb[j][i+7] > 0.0f);
            }
            for (; i < 256; i++) {
                cond[j][i] = (bb[j][i] > 0.0f);
            }
        }

        // Compute per-row counts with unrolling
        for (int j = 0; j < 256; j++) {
            int cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0;
            int cnt4 = 0, cnt5 = 0, cnt6 = 0, cnt7 = 0;
            int i = 0;
            for (; i <= 256 - 8; i += 8) {
                cnt0 += cond[j][i+0];
                cnt1 += cond[j][i+1];
                cnt2 += cond[j][i+2];
                cnt3 += cond[j][i+3];
                cnt4 += cond[j][i+4];
                cnt5 += cond[j][i+5];
                cnt6 += cond[j][i+6];
                cnt7 += cond[j][i+7];
            }
            int cnt = (cnt0 + cnt1) + (cnt2 + cnt3) + (cnt4 + cnt5) + (cnt6 + cnt7);
            for (; i < 256; i++) {
                cnt += cond[j][i];
            }
            row_count[j] = cnt;
        }

        // Prefix sum (sequential)
        row_start[0] = 0;
        for (int j = 0; j < 256; j++) {
            row_start[j+1] = row_start[j] + row_count[j];
        }

        // Scatter values
        for (int j = 0; j < 256; j++) {
            int base = row_start[j];
            for (int i = 0; i < 256; i++) {
                flat_2d_array[base] = aa[j][i];
                base += cond[j][i];
            }
        }
    }
}
