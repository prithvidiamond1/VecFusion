#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s3113(int iterations, int LEN_1D, float* a)
{
    float max = fabsf(a[0]);
    for (int i = 1; i < LEN_1D; i++) {
        float abs_val = fabsf(a[i]);
        max = abs_val > max ? abs_val : max;
    }
    return max;
}

        #include <math.h>

float vectorized_s3113(int iterations, int LEN_1D, float* a)
{
    typedef float float4 __attribute__((vector_size(16)));

    float max = fabsf(a[0]);

    int i = 1;
    int vec_len = 4;

    // Process chunks of 4 using vector types
    // First, build up a vector of 4 max values
    float4 vmax = {fabsf(a[0]), fabsf(a[0]), fabsf(a[0]), fabsf(a[0])};

    // Reset i to 0 for vector processing from start
    i = 0;
    int limit = LEN_1D - (LEN_1D % vec_len);

    for (i = 0; i < limit; i += vec_len) {
        float4 va;
        va[0] = fabsf(a[i + 0]);
        va[1] = fabsf(a[i + 1]);
        va[2] = fabsf(a[i + 2]);
        va[3] = fabsf(a[i + 3]);

        // Element-wise max
        vmax[0] = va[0] > vmax[0] ? va[0] : vmax[0];
        vmax[1] = va[1] > vmax[1] ? va[1] : vmax[1];
        vmax[2] = va[2] > vmax[2] ? va[2] : vmax[2];
        vmax[3] = va[3] > vmax[3] ? va[3] : vmax[3];
    }

    // Reduce vector max to scalar
    max = vmax[0];
    if (vmax[1] > max) max = vmax[1];
    if (vmax[2] > max) max = vmax[2];
    if (vmax[3] > max) max = vmax[3];

    // Scalar tail
    for (; i < LEN_1D; i++) {
        float abs_val = fabsf(a[i]);
        if (abs_val > max) max = abs_val;
    }

    return max;
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
                ret_scalar = s3113(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s3113(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
