#include <stdint.h>

typedef float float32_t;

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.0f;
        for (int i = 0; i < 256; i += 4) {
            float32_t a_vec[4] = {a[i], a[i+1], a[i+2], a[i+3]};
            float32_t d_vec[4] = {d[i], d[i+1], d[i+2], d[i+3]};
            float32_t c_vec[4] = {c[i], c[i+1], c[i+2], c[i+3]};
            float32_t aa0_vec[4] = {aa[0][i], aa[0][i+1], aa[0][i+2], aa[0][i+3]};

            float32_t s_vec[4];
            float32_t prev_s = s;
            for (int j = 0; j < 4; j++) {
                s_vec[j] = (a_vec[j] > 0.0f) ? (d_vec[j] * d_vec[j]) : prev_s;
                prev_s = s_vec[j];
            }

            for (int j = 0; j < 4; j++) {
                b[i + j] = s_vec[j] * c_vec[j] + d_vec[j];
                e[i + j] = (s_vec[j] + 1.0f) * aa0_vec[j];
            }

            for (int j = 0; j < 4; j++) {
                if (a[i + j] > 0.0f) {
                    s = d[i + j] * d[i + j];
                }
            }
        }
    }
}
