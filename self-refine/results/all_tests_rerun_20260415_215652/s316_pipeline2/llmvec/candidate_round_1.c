#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        float x0 = a[0];
        float x1 = a[0];
        float x2 = a[0];
        float x3 = a[0];

        int i;
        for (i = 1; i < LEN_1D - 3; i += 4) {
            x0 = x0 < a[i]   ? x0 : a[i];
            x1 = x1 < a[i+1] ? x1 : a[i+1];
            x2 = x2 < a[i+2] ? x2 : a[i+2];
            x3 = x3 < a[i+3] ? x3 : a[i+3];
        }
        for (; i < LEN_1D; i++) {
            x0 = x0 < a[i] ? x0 : a[i];
        }
        x0 = x0 < x1 ? x0 : x1;
        x2 = x2 < x3 ? x2 : x3;
        x = x0 < x2 ? x0 : x2;
    }
    return x;
}

        #include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

float vectorized_s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        // Initialize 4-wide SIMD accumulators with a[0]
        float4 vmin = {a[0], a[0], a[0], a[0]};

        int i;
        // Process 16 elements at a time using 4 vector lanes
        for (i = 1; i + 15 < LEN_1D; i += 16) {
            float4 v0 = {a[i],    a[i+1],  a[i+2],  a[i+3]};
            float4 v1 = {a[i+4],  a[i+5],  a[i+6],  a[i+7]};
            float4 v2 = {a[i+8],  a[i+9],  a[i+10], a[i+11]};
            float4 v3 = {a[i+12], a[i+13], a[i+14], a[i+15]};

            // element-wise min using ternary (compiler will use fmin/cmov)
            float4 m0 = vmin < v0 ? vmin : v0;
            float4 m1 = v1 < v2 ? v1 : v2;
            float4 m2 = m0 < m1 ? m0 : m1;
            float4 m3 = m2 < v3 ? m2 : v3;
            vmin = m3;
        }

        // Process remaining groups of 4
        for (; i + 3 < LEN_1D; i += 4) {
            float4 v = {a[i], a[i+1], a[i+2], a[i+3]};
            vmin = vmin < v ? vmin : v;
        }

        // Reduce the 4-lane vector to scalar
        float x0 = vmin[0];
        float x1 = vmin[1];
        float x2 = vmin[2];
        float x3 = vmin[3];

        // Scalar tail
        for (; i < LEN_1D; i++) {
            x0 = x0 < a[i] ? x0 : a[i];
        }

        x0 = x0 < x1 ? x0 : x1;
        x2 = x2 < x3 ? x2 : x3;
        x = x0 < x2 ? x0 : x2;
    }
    return x;
}

        static uint32_t next_u32(uint32_t *state) {
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }

        static void fill_i32(int *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }
        }

        static void fill_f32(float *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
            }
        }

        static void fill_f64(double *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
            }
        }

        int main(void) {
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s316(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s316(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
