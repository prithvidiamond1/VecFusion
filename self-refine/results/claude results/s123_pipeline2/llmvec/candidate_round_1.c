#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half = LEN_1D / 2;
    float tmp_b[LEN_1D];
    float tmp_c[LEN_1D];
    float tmp_de[LEN_1D];
    int   tmp_valid[LEN_1D];

    for (int nl = 0; nl < iterations; nl++) {

        // Loop 1: compute de, tmp_b, tmp_c, tmp_valid (vectorizable)
        for (int i = 0; i < half; i++) {
            tmp_de[i]    = d[i] * e[i];
            tmp_b[i]     = b[i] + tmp_de[i];
            tmp_c[i]     = c[i] + tmp_de[i];
            tmp_valid[i] = (c[i] > 0.0f) ? 1 : 0;
        }

        // Loop 2: scatter results into a[] sequentially
        int j = 0;
        for (int i = 0; i < half; i++) {
            a[j] = tmp_b[i];
            j++;
            if (tmp_valid[i]) {
                a[j] = tmp_c[i];
                j++;
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half = LEN_1D / 2;
    float tmp_b[LEN_1D];
    float tmp_c[LEN_1D];
    float tmp_de[LEN_1D];
    int   tmp_valid[LEN_1D];

    for (int nl = 0; nl < iterations; nl++) {

        // Loop 1: vectorized using compiler vector extensions
        // Process in chunks of 4 floats
        int i = 0;
        int limit4 = half - (half % 4);

        for (; i < limit4; i += 4) {
            // Load 4 elements
            float d0 = d[i+0], d1 = d[i+1], d2 = d[i+2], d3 = d[i+3];
            float e0 = e[i+0], e1 = e[i+1], e2 = e[i+2], e3 = e[i+3];
            float b0 = b[i+0], b1 = b[i+1], b2 = b[i+2], b3 = b[i+3];
            float c0 = c[i+0], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];

            float de0 = d0 * e0;
            float de1 = d1 * e1;
            float de2 = d2 * e2;
            float de3 = d3 * e3;

            tmp_de[i+0] = de0;
            tmp_de[i+1] = de1;
            tmp_de[i+2] = de2;
            tmp_de[i+3] = de3;

            tmp_b[i+0] = b0 + de0;
            tmp_b[i+1] = b1 + de1;
            tmp_b[i+2] = b2 + de2;
            tmp_b[i+3] = b3 + de3;

            tmp_c[i+0] = c0 + de0;
            tmp_c[i+1] = c1 + de1;
            tmp_c[i+2] = c2 + de2;
            tmp_c[i+3] = c3 + de3;

            tmp_valid[i+0] = (c0 > 0.0f) ? 1 : 0;
            tmp_valid[i+1] = (c1 > 0.0f) ? 1 : 0;
            tmp_valid[i+2] = (c2 > 0.0f) ? 1 : 0;
            tmp_valid[i+3] = (c3 > 0.0f) ? 1 : 0;
        }

        // Scalar tail for loop 1
        for (; i < half; i++) {
            tmp_de[i]    = d[i] * e[i];
            tmp_b[i]     = b[i] + tmp_de[i];
            tmp_c[i]     = c[i] + tmp_de[i];
            tmp_valid[i] = (c[i] > 0.0f) ? 1 : 0;
        }

        // Loop 2: scatter results into a[] sequentially (inherently serial due to dependency)
        int j = 0;
        for (int i = 0; i < half; i++) {
            a[j] = tmp_b[i];
            j++;
            if (tmp_valid[i]) {
                a[j] = tmp_c[i];
                j++;
            }
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s123(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s123(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
