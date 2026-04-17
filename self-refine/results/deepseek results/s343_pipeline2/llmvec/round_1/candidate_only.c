#include <string.h>

void vectorized_s343(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int outer_loops = 10 * (iterations / 256);

    for (int nl = 0; nl < outer_loops; nl++) {
        int k = -1;

        for (int i = 0; i < 256; i++) {
            int mask[256];
            int prefix_sum[256];
            int count = 0;

            // Unrolled mask computation (4-wide)
            int j = 0;
            for (; j <= 252; j += 4) {
                int m0 = (bb[j+0][i] > 0.0f);
                int m1 = (bb[j+1][i] > 0.0f);
                int m2 = (bb[j+2][i] > 0.0f);
                int m3 = (bb[j+3][i] > 0.0f);
                mask[j+0] = m0;
                mask[j+1] = m1;
                mask[j+2] = m2;
                mask[j+3] = m3;
                count += m0 + m1 + m2 + m3;
                prefix_sum[j+0] = count - (m1 + m2 + m3);
                prefix_sum[j+1] = count - (m2 + m3);
                prefix_sum[j+2] = count - m3;
                prefix_sum[j+3] = count;
            }
            // Scalar tail
            for (; j < 256; j++) {
                mask[j] = (bb[j][i] > 0.0f);
                count += mask[j];
                prefix_sum[j] = count;
            }

            // Write selected elements
            j = 0;
            for (; j <= 252; j += 4) {
                if (mask[j+0]) {
                    flat_2d_array[k + prefix_sum[j+0]] = aa[j+0][i];
                }
                if (mask[j+1]) {
                    flat_2d_array[k + prefix_sum[j+1]] = aa[j+1][i];
                }
                if (mask[j+2]) {
                    flat_2d_array[k + prefix_sum[j+2]] = aa[j+2][i];
                }
                if (mask[j+3]) {
                    flat_2d_array[k + prefix_sum[j+3]] = aa[j+3][i];
                }
            }
            for (; j < 256; j++) {
                if (mask[j]) {
                    flat_2d_array[k + prefix_sum[j]] = aa[j][i];
                }
            }

            k += count;
        }
    }
}
