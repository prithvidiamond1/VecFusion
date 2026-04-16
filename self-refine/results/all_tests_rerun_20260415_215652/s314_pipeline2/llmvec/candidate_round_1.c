#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s314(int iterations, int LEN_1D, float* a)
{
    float x = a[0];
    for (int i = 1; i < LEN_1D; i++) {
        float ai = a[i];
        x = ai > x ? ai : x;
    }
    return x;
}

        #include <stddef.h>

float vectorized_s314(int iterations, int LEN_1D, float* a)
{
    typedef float float4 __attribute__((vector_size(16)));

    float x = a[0];

    int i = 1;

    // We'll use 4-wide SIMD vectors
    // Initialize vector with x in all lanes
    float4 vmax = {x, x, x, x};

    // Process chunks of 4 starting from index 1
    // We need to be careful: start at i=1, so we process until we can fill a full vector
    // For simplicity, let's start vector processing from index 0 and fix up at end
    // Actually let's just do scalar until we hit alignment, then vector

    // Simple approach: process 4 at a time from i=1
    // We'll accumulate 4 separate max lanes
    float4 vmax0 = {x, x, x, x};
    float4 vmax1 = {x, x, x, x};
    float4 vmax2 = {x, x, x, x};
    float4 vmax3 = {x, x, x, x};

    int limit = LEN_1D - 16 + 1;
    for (; i < limit; i += 16) {
        float4 v0, v1, v2, v3;
        // Load 4 floats into each vector
        __builtin_memcpy(&v0, &a[i],    sizeof(float4));
        __builtin_memcpy(&v1, &a[i+4],  sizeof(float4));
        __builtin_memcpy(&v2, &a[i+8],  sizeof(float4));
        __builtin_memcpy(&v3, &a[i+12], sizeof(float4));

        // Element-wise max using ternary (compiler should vectorize)
        vmax0 = v0 > vmax0 ? v0 : vmax0;
        vmax1 = v1 > vmax1 ? v1 : vmax1;
        vmax2 = v2 > vmax2 ? v2 : vmax2;
        vmax3 = v3 > vmax3 ? v3 : vmax3;
    }

    // Merge the 4 accumulators
    vmax0 = vmax0 > vmax1 ? vmax0 : vmax1;
    vmax2 = vmax2 > vmax3 ? vmax2 : vmax3;
    vmax0 = vmax0 > vmax2 ? vmax0 : vmax2;

    // Process remaining chunks of 4
    limit = LEN_1D - 4 + 1;
    for (; i < limit; i += 4) {
        float4 v0;
        __builtin_memcpy(&v0, &a[i], sizeof(float4));
        vmax0 = v0 > vmax0 ? v0 : vmax0;
    }

    // Reduce vmax0 to scalar
    float tmp[4];
    __builtin_memcpy(tmp, &vmax0, sizeof(float4));
    for (int j = 0; j < 4; j++) {
        if (tmp[j] > x) x = tmp[j];
    }

    // Scalar tail
    for (; i < LEN_1D; i++) {
        float ai = a[i];
        x = ai > x ? ai : x;
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
                ret_scalar = s314(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s314(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
