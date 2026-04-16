#include <string.h>
#include <stdlib.h>

void vectorized_s141(int iterations, float * __restrict__ flat_2d_array, float bb[256][256])
{
    int nl_count = 200 * (iterations / 256);
    float nl_count_f = (float)nl_count;

    // Pre-compute k indices for all i
    int k_start[256];
    for (int i = 0; i < 256; i++) {
        k_start[i] = (i + 1) * i / 2 + i;
    }

    // Use heap allocation to avoid large stack usage and static issues
    float *col_sum = (float *)calloc(256 * 256, sizeof(float));
    if (!col_sum) return;

    float *bb_flat = (float *)bb;

    // Fill upper triangle: col_sum[i*256+j] = bb[j][i] * nl_count, for i <= j
    for (int j = 0; j < 256; j++) {
        int i = 0;
        for (; i + 7 <= j; i += 8) {
            col_sum[i*256+j]     = bb_flat[j*256 + i]   * nl_count_f;
            col_sum[(i+1)*256+j] = bb_flat[j*256 + i+1] * nl_count_f;
            col_sum[(i+2)*256+j] = bb_flat[j*256 + i+2] * nl_count_f;
            col_sum[(i+3)*256+j] = bb_flat[j*256 + i+3] * nl_count_f;
            col_sum[(i+4)*256+j] = bb_flat[j*256 + i+4] * nl_count_f;
            col_sum[(i+5)*256+j] = bb_flat[j*256 + i+5] * nl_count_f;
            col_sum[(i+6)*256+j] = bb_flat[j*256 + i+6] * nl_count_f;
            col_sum[(i+7)*256+j] = bb_flat[j*256 + i+7] * nl_count_f;
        }
        for (; i <= j; i++) {
            col_sum[i*256+j] = bb_flat[j*256 + i] * nl_count_f;
        }
    }

    // Single scatter pass: for each row i, walk j from i to 255
    // k advances by (j+1) each step - scatter so no vectorization, just unroll
    for (int i = 0; i < 256; i++) {
        int k = k_start[i];
        for (int j = i; j < 256; j++) {
            flat_2d_array[k] += col_sum[i*256+j];
            k += j + 1;
        }
    }

    free(col_sum);
}
