#include <stdint.h>
#include <string.h>

float vectorized_s3110(int iterations, float aa[256][256])
{
    int xindex, yindex;
    float max, chksum;

    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        for (int i = 0; i < 256; i++) {
            // Process row i in chunks of 8
            int j = 0;
            for (; j <= 256 - 8; j += 8) {
                float v0 = aa[i][j+0];
                float v1 = aa[i][j+1];
                float v2 = aa[i][j+2];
                float v3 = aa[i][j+3];
                float v4 = aa[i][j+4];
                float v5 = aa[i][j+5];
                float v6 = aa[i][j+6];
                float v7 = aa[i][j+7];

                if (v0 > max) { max = v0; xindex = i; yindex = j+0; }
                if (v1 > max) { max = v1; xindex = i; yindex = j+1; }
                if (v2 > max) { max = v2; xindex = i; yindex = j+2; }
                if (v3 > max) { max = v3; xindex = i; yindex = j+3; }
                if (v4 > max) { max = v4; xindex = i; yindex = j+4; }
                if (v5 > max) { max = v5; xindex = i; yindex = j+5; }
                if (v6 > max) { max = v6; xindex = i; yindex = j+6; }
                if (v7 > max) { max = v7; xindex = i; yindex = j+7; }
            }
            // Scalar tail
            for (; j < 256; j++) {
                if (aa[i][j] > max) {
                    max = aa[i][j];
                    xindex = i;
                    yindex = j;
                }
            }
        }
        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}
