#include <stddef.h>

typedef float float_vec __attribute__((__vector_size__(16)));

void vectorized_s211(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        // Vectorize with 4-wide SIMD
        for (; i + 4 <= LEN_1D - 1; i += 4) {
            // Load b[i-1..i+2]
            float_vec vb_prev = {b[i-1], b[i], b[i+1], b[i+2]};
            // Load c[i..i+3]
            float_vec vc = {c[i], c[i+1], c[i+2], c[i+3]};
            // Load d[i..i+3]
            float_vec vd = {d[i], d[i+1], d[i+2], d[i+3]};
            // Load e[i..i+3]
            float_vec ve = {e[i], e[i+1], e[i+2], e[i+3]};
            // Load b[i+1..i+4]
            float_vec vb_next = {b[i+1], b[i+2], b[i+3], b[i+4]};

            // a[i] = b[i-1] + c[i] * d[i]
            float_vec va = vb_prev + vc * vd;
            a[i] = va[0];
            a[i+1] = va[1];
            a[i+2] = va[2];
            a[i+3] = va[3];

            // b[i] = b[i+1] - e[i] * d[i]
            float_vec vb_new = vb_next - ve * vd;
            b[i] = vb_new[0];
            b[i+1] = vb_new[1];
            b[i+2] = vb_new[2];
            b[i+3] = vb_new[3];
        }
        // Scalar cleanup
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
            b[i] = b[i + 1] - e[i] * d[i];
        }
    }
}
