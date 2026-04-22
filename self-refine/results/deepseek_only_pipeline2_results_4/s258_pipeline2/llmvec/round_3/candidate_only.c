#include <stdint.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float aa0[256];
    for (int i = 0; i < 256; ++i) {
        aa0[i] = aa[0][i];
    }

    for (int nl = 0; nl < iterations; nl++) {
        float temp_s[256];
        float d_sq[256];

        // Vectorized precomputation of d_sq
        int i = 0;
        for (; i <= 252; i += 4) {
            d_sq[i] = d[i] * d[i];
            d_sq[i+1] = d[i+1] * d[i+1];
            d_sq[i+2] = d[i+2] * d[i+2];
            d_sq[i+3] = d[i+3] * d[i+3];
        }
        for (; i < 256; ++i) {
            d_sq[i] = d[i] * d[i];
        }

        // First loop: compute s values with dependency - keep scalar for correctness
        float s = 0.0f;
        for (i = 0; i < 256; ++i) {
            float mask = (a[i] > 0.0f) ? 1.0f : 0.0f;
            s = mask * d_sq[i] + (1.0f - mask) * s;
            temp_s[i] = s;
            a[i] = s;
        }

        // Second loop: vectorized computation of b[i] and e[i]
        i = 0;
        for (; i <= 252; i += 4) {
            float s_val0 = temp_s[i];
            float s_val1 = temp_s[i+1];
            float s_val2 = temp_s[i+2];
            float s_val3 = temp_s[i+3];

            b[i] = s_val0 * c[i] + d[i];
            b[i+1] = s_val1 * c[i+1] + d[i+1];
            b[i+2] = s_val2 * c[i+2] + d[i+2];
            b[i+3] = s_val3 * c[i+3] + d[i+3];

            e[i] = (s_val0 + 1.0f) * aa0[i];
            e[i+1] = (s_val1 + 1.0f) * aa0[i+1];
            e[i+2] = (s_val2 + 1.0f) * aa0[i+2];
            e[i+3] = (s_val3 + 1.0f) * aa0[i+3];
        }
        for (; i < 256; ++i) {
            float s_val = temp_s[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa0[i];
        }
    }
}
