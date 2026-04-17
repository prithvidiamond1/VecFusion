#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s482(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Find the break point first
        int break_point = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_point = i + 1;
                break;
            }
        }
        // Now vectorize the update loop with known bounds
        for (int i = 0; i < break_point; i++) {
            a[i] += b[i] * c[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Find the break point first (scalar, data-dependent)
        int break_point = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_point = i + 1;
                break;
            }
        }

        // Vectorized update loop using compiler vector types
        typedef float float4 __attribute__((vector_size(16)));

        int i = 0;
        int vec_len = break_point & ~3; // round down to multiple of 4

        for (; i < vec_len; i += 4) {
            float4 va, vb, vc;
            // Load 4 floats manually
            va[0] = a[i];   va[1] = a[i+1];   va[2] = a[i+2];   va[3] = a[i+3];
            vb[0] = b[i];   vb[1] = b[i+1];   vb[2] = b[i+2];   vb[3] = b[i+3];
            vc[0] = c[i];   vc[1] = c[i+1];   vc[2] = c[i+2];   vc[3] = c[i+3];

            va = va + vb * vc;

            a[i]   = va[0];
            a[i+1] = va[1];
            a[i+2] = va[2];
            a[i+3] = va[3];
        }

        // Scalar tail
        for (; i < break_point; i++) {
            a[i] += b[i] * c[i];
        }
    }
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s482(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s482(iterations, LEN_1D, a_vector, b_vector, c_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
