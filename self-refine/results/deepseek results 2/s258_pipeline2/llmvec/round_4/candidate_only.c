#include <string.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.f;

        // First loop: compute s_seq serially (carries dependency, must stay scalar)
        float s_seq[256];
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.f) {
                s = d[i] * d[i];
            }
            s_seq[i] = s;
        }

        // Second loop: vectorize b and e, use scalar for aa diagonal
        typedef float float8 __attribute__((vector_size(32)));

        int i = 0;
        for (; i <= 256 - 8; i += 8) {
            float8 sv, cv, dv;

            memcpy(&sv, &s_seq[i], 32);
            memcpy(&cv, &c[i], 32);
            memcpy(&dv, &d[i], 32);

            // a[i] = s_val * c[i] + d[i]
            float8 av_out = sv * cv + dv;
            memcpy(&a[i], &av_out, 32);

            // e[i] = (s_val + 1.0f) * aa[i][i]  -- diagonal gather, done scalar
            for (int j = i; j < i + 8; ++j) {
                e[j] = (s_seq[j] + 1.0f) * aa[j][j];
            }
        }

        // Scalar tail
        for (; i < 256; ++i) {
            float s_val = s_seq[i];
            a[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa[i][i];
        }
    }
}
