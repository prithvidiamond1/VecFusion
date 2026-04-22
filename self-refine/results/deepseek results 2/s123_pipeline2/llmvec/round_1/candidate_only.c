#include <stdint.h>
#include <string.h>

void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half_len = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorizable: compute de[i] and mask[i]
        float de[half_len];
        int mask[half_len];

        int i = 0;
        // Process in chunks of 8
        for (; i <= half_len - 8; i += 8) {
            de[i+0] = d[i+0] * e[i+0];
            de[i+1] = d[i+1] * e[i+1];
            de[i+2] = d[i+2] * e[i+2];
            de[i+3] = d[i+3] * e[i+3];
            de[i+4] = d[i+4] * e[i+4];
            de[i+5] = d[i+5] * e[i+5];
            de[i+6] = d[i+6] * e[i+6];
            de[i+7] = d[i+7] * e[i+7];

            mask[i+0] = (c[i+0] > 0.0f) ? 1 : 0;
            mask[i+1] = (c[i+1] > 0.0f) ? 1 : 0;
            mask[i+2] = (c[i+2] > 0.0f) ? 1 : 0;
            mask[i+3] = (c[i+3] > 0.0f) ? 1 : 0;
            mask[i+4] = (c[i+4] > 0.0f) ? 1 : 0;
            mask[i+5] = (c[i+5] > 0.0f) ? 1 : 0;
            mask[i+6] = (c[i+6] > 0.0f) ? 1 : 0;
            mask[i+7] = (c[i+7] > 0.0f) ? 1 : 0;
        }
        // Scalar tail
        for (; i < half_len; i++) {
            de[i] = d[i] * e[i];
            mask[i] = (c[i] > 0.0f) ? 1 : 0;
        }

        // Prefix sum to compute write positions (inherently sequential)
        int write_pos[half_len];
        int current_j = -1;
        for (i = 0; i < half_len; i++) {
            current_j++;
            write_pos[i] = current_j;
            if (mask[i]) {
                current_j++;
            }
        }

        // First write: vectorizable (scatter with computed indices)
        // Unrolled for better vectorization hints
        i = 0;
        for (; i <= half_len - 8; i += 8) {
            a[write_pos[i+0]] = b[i+0] + de[i+0];
            a[write_pos[i+1]] = b[i+1] + de[i+1];
            a[write_pos[i+2]] = b[i+2] + de[i+2];
            a[write_pos[i+3]] = b[i+3] + de[i+3];
            a[write_pos[i+4]] = b[i+4] + de[i+4];
            a[write_pos[i+5]] = b[i+5] + de[i+5];
            a[write_pos[i+6]] = b[i+6] + de[i+6];
            a[write_pos[i+7]] = b[i+7] + de[i+7];
        }
        for (; i < half_len; i++) {
            a[write_pos[i]] = b[i] + de[i];
        }

        // Second write: conditional scatter
        i = 0;
        for (; i <= half_len - 8; i += 8) {
            if (mask[i+0]) a[write_pos[i+0] + 1] = c[i+0] + de[i+0];
            if (mask[i+1]) a[write_pos[i+1] + 1] = c[i+1] + de[i+1];
            if (mask[i+2]) a[write_pos[i+2] + 1] = c[i+2] + de[i+2];
            if (mask[i+3]) a[write_pos[i+3] + 1] = c[i+3] + de[i+3];
            if (mask[i+4]) a[write_pos[i+4] + 1] = c[i+4] + de[i+4];
            if (mask[i+5]) a[write_pos[i+5] + 1] = c[i+5] + de[i+5];
            if (mask[i+6]) a[write_pos[i+6] + 1] = c[i+6] + de[i+6];
            if (mask[i+7]) a[write_pos[i+7] + 1] = c[i+7] + de[i+7];
        }
        for (; i < half_len; i++) {
            if (mask[i]) a[write_pos[i] + 1] = c[i] + de[i];
        }
    }
}
