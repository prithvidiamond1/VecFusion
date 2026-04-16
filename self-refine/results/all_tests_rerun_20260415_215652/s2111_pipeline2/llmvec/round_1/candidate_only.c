#include <stdint.h>

void vectorized_s2111(int iterations, float aa[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int d = 2; d <= 510; d++) {
            int j_start = (d - 255 > 1) ? d - 255 : 1;
            int j_end   = (d - 1 < 255) ? d - 1 : 255;
            int len = j_end - j_start + 1;

            // Unroll by 4 for vector-friendly access
            int j = j_start;
            int limit4 = j_start + (len & ~3);

            for (; j < limit4; j += 4) {
                int i0 = d - j;
                int i1 = d - (j+1);
                int i2 = d - (j+2);
                int i3 = d - (j+3);

                float v0 = (aa[j  ][i0-1] + aa[j-1  ][i0]) / 1.9f;
                float v1 = (aa[j+1][i1-1] + aa[j  ][i1]) / 1.9f;
                float v2 = (aa[j+2][i2-1] + aa[j+1][i2]) / 1.9f;
                float v3 = (aa[j+3][i3-1] + aa[j+2][i3]) / 1.9f;

                aa[j  ][i0] = v0;
                aa[j+1][i1] = v1;
                aa[j+2][i2] = v2;
                aa[j+3][i3] = v3;
            }

            // Scalar tail
            for (; j <= j_end; j++) {
                int i = d - j;
                aa[j][i] = (aa[j][i-1] + aa[j-1][i]) / 1.9f;
            }
        }
    }
}
