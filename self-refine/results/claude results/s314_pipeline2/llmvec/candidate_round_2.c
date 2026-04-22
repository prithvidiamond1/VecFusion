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

typedef float float4 __attribute__((vector_size(16)));

float vectorized_s314(int iterations, int LEN_1D, float* a)
{
    float x = a[0];

    int i = 1;

    float4 vmax0 = {x, x, x, x};
    float4 vmax1 = {x, x, x, x};
    float4 vmax2 = {x, x, x, x};
    float4 vmax3 = {x, x, x, x};

    int limit = LEN_1D - 16 + 1;
    for (; i < limit; i += 16) {
        float4 v0, v1, v2, v3;
        __builtin_memcpy(&v0, &a[i],    sizeof(float4));
        __builtin_memcpy(&v1, &a[i+4],  sizeof(float4));
        __builtin_memcpy(&v2, &a[i+8],  sizeof(float4));
        __builtin_memcpy(&v3, &a[i+12], sizeof(float4));

        vmax0 = __builtin_elementwise_max(v0, vmax0);
        vmax1 = __builtin_elementwise_max(v1, vmax1);
        vmax2 = __builtin_elementwise_max(v2, vmax2);
        vmax3 = __builtin_elementwise_max(v3, vmax3);
    }

    vmax0 = __builtin_elementwise_max(vmax0, vmax1);
    vmax2 = __builtin_elementwise_max(vmax2, vmax3);
    vmax0 = __builtin_elementwise_max(vmax0, vmax2);

    limit = LEN_1D - 4 + 1;
    for (; i < limit; i += 4) {
        float4 v0;
        __builtin_memcpy(&v0, &a[i], sizeof(float4));
        vmax0 = __builtin_elementwise_max(v0, vmax0);
    }

    float tmp[4];
    __builtin_memcpy(tmp, &vmax0, sizeof(float4));
    for (int j = 0; j < 4; j++) {
        if (tmp[j] > x) x = tmp[j];
    }

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
