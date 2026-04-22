#include <string.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: b[i] = b[i+1] - e[i] * d[i], for i = 1 .. LEN_1D-2
        // b[i+1] are the original values read before any writes in this loop.
        // Since we write b[i] and read b[i+1], and i goes forward, b[i+1] hasn't
        // been written yet when we read it. So we can vectorize directly.
        int i = 1;
        int len = LEN_1D - 2; // number of iterations: i from 1 to LEN_1D-2 inclusive

        // Vector width: 8 floats
        #define VW 8
        typedef float float8 __attribute__((vector_size(VW * sizeof(float))));

        int vec_end = 1 + (len / VW) * VW;
        for (; i < vec_end; i += VW) {
            float8 b_next, ei, di, res;
            __builtin_memcpy(&b_next, &b[i+1], sizeof(float8));
            __builtin_memcpy(&ei, &e[i], sizeof(float8));
            __builtin_memcpy(&di, &d[i], sizeof(float8));
            res = b_next - ei * di;
            __builtin_memcpy(&b[i], &res, sizeof(float8));
        }
        // Scalar tail
        for (; i < LEN_1D - 1; i++) {
            b[i] = b[i+1] - e[i] * d[i];
        }

        // Second loop: a[i] = b[i-1] + c[i] * d[i], for i = 1 .. LEN_1D-2
        i = 1;
        vec_end = 1 + (len / VW) * VW;
        for (; i < vec_end; i += VW) {
            float8 b_prev, ci, di, res;
            __builtin_memcpy(&b_prev, &b[i-1], sizeof(float8));
            __builtin_memcpy(&ci, &c[i], sizeof(float8));
            __builtin_memcpy(&di, &d[i], sizeof(float8));
            res = b_prev + ci * di;
            __builtin_memcpy(&a[i], &res, sizeof(float8));
        }
        // Scalar tail
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }

        #undef VW
    }
}
