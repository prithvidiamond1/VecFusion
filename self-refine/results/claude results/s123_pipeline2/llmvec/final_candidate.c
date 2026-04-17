#include <stdint.h>
#include <string.h>

void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half = LEN_1D / 2;
    float tmp_b[LEN_1D];
    float tmp_c[LEN_1D];
    float tmp_de[LEN_1D];
    int   tmp_valid[LEN_1D];

    for (int nl = 0; nl < iterations; nl++) {

        // Loop 1: vectorized using compiler vector extensions
        // Process in chunks of 4 floats
        int i = 0;
        int limit4 = half - (half % 4);

        for (; i < limit4; i += 4) {
            // Load 4 elements
            float d0 = d[i+0], d1 = d[i+1], d2 = d[i+2], d3 = d[i+3];
            float e0 = e[i+0], e1 = e[i+1], e2 = e[i+2], e3 = e[i+3];
            float b0 = b[i+0], b1 = b[i+1], b2 = b[i+2], b3 = b[i+3];
            float c0 = c[i+0], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];

            float de0 = d0 * e0;
            float de1 = d1 * e1;
            float de2 = d2 * e2;
            float de3 = d3 * e3;

            tmp_de[i+0] = de0;
            tmp_de[i+1] = de1;
            tmp_de[i+2] = de2;
            tmp_de[i+3] = de3;

            tmp_b[i+0] = b0 + de0;
            tmp_b[i+1] = b1 + de1;
            tmp_b[i+2] = b2 + de2;
            tmp_b[i+3] = b3 + de3;

            tmp_c[i+0] = c0 + de0;
            tmp_c[i+1] = c1 + de1;
            tmp_c[i+2] = c2 + de2;
            tmp_c[i+3] = c3 + de3;

            tmp_valid[i+0] = (c0 > 0.0f) ? 1 : 0;
            tmp_valid[i+1] = (c1 > 0.0f) ? 1 : 0;
            tmp_valid[i+2] = (c2 > 0.0f) ? 1 : 0;
            tmp_valid[i+3] = (c3 > 0.0f) ? 1 : 0;
        }

        // Scalar tail for loop 1
        for (; i < half; i++) {
            tmp_de[i]    = d[i] * e[i];
            tmp_b[i]     = b[i] + tmp_de[i];
            tmp_c[i]     = c[i] + tmp_de[i];
            tmp_valid[i] = (c[i] > 0.0f) ? 1 : 0;
        }

        // Loop 2: scatter results into a[] sequentially (inherently serial due to dependency)
        int j = 0;
        for (int i = 0; i < half; i++) {
            a[j] = tmp_b[i];
            j++;
            if (tmp_valid[i]) {
                a[j] = tmp_c[i];
                j++;
            }
        }
    }
}
