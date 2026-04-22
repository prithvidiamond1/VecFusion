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

        // Second loop: vectorize using Clang vector extensions
        float* aa_row = aa[0];

        typedef float float8 __attribute__((vector_size(32)));

        int i = 0;
        for (; i <= 256 - 8; i += 8) {
            float8 sv, cv, dv, aav, bv, ev;

            memcpy(&sv, &s_seq[i], 32);
            memcpy(&cv, &c[i], 32);
            memcpy(&dv, &d[i], 32);
            memcpy(&aav, &aa_row[i], 32);

            // b[i] = s_val * c[i] + d[i]
            bv = sv * cv + dv;
            // e[i] = (s_val + 1.0f) * aa_row[i]
            float8 ones = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
            ev = (sv + ones) * aav;

            memcpy(&b[i], &bv, 32);
            memcpy(&e[i], &ev, 32);
        }

        // Scalar tail
        for (; i < 256; ++i) {
            float s_val = s_seq[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa_row[i];
        }
    }
}
