#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half_len = LEN_1D/2;

    for (int nl = 0; nl < iterations; nl++) {
        // Precompute d[i]*e[i] and condition mask
        float de[half_len];
        int mask[half_len];

        // Vectorizable loop: independent computations
        for (int i = 0; i < half_len; i++) {
            de[i] = d[i] * e[i];
            mask[i] = (c[i] > (float)0.);
        }

        // Compute write positions using prefix sum
        int write_pos[half_len];
        int current_j = -1;
        for (int i = 0; i < half_len; i++) {
            current_j++;
            write_pos[i] = current_j;
            if (mask[i]) {
                current_j++;
            }
        }

        // First write: always write b[i] + de[i]
        for (int i = 0; i < half_len; i++) {
            a[write_pos[i]] = b[i] + de[i];
        }

        // Second write: conditional write c[i] + de[i]
        for (int i = 0; i < half_len; i++) {
            if (mask[i]) {
                a[write_pos[i] + 1] = c[i] + de[i];
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half_len = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorizable: compute de[i] and mask[i]
        float de[half_len];
        int mask[half_len];

        int i = 0;
        // Process in chunks of 8
        for (; i <= half_len - 8; i += 8) {
            de[i+0] = d[i+0] * e[i+0];
            de[i+1] = d[i+1] * e[i+1];
            de[i+2] = d[i+2] * e[i+2];
            de[i+3] = d[i+3] * e[i+3];
            de[i+4] = d[i+4] * e[i+4];
            de[i+5] = d[i+5] * e[i+5];
            de[i+6] = d[i+6] * e[i+6];
            de[i+7] = d[i+7] * e[i+7];

            mask[i+0] = (c[i+0] > 0.0f) ? 1 : 0;
            mask[i+1] = (c[i+1] > 0.0f) ? 1 : 0;
            mask[i+2] = (c[i+2] > 0.0f) ? 1 : 0;
            mask[i+3] = (c[i+3] > 0.0f) ? 1 : 0;
            mask[i+4] = (c[i+4] > 0.0f) ? 1 : 0;
            mask[i+5] = (c[i+5] > 0.0f) ? 1 : 0;
            mask[i+6] = (c[i+6] > 0.0f) ? 1 : 0;
            mask[i+7] = (c[i+7] > 0.0f) ? 1 : 0;
        }
        // Scalar tail
        for (; i < half_len; i++) {
            de[i] = d[i] * e[i];
            mask[i] = (c[i] > 0.0f) ? 1 : 0;
        }

        // Prefix sum to compute write positions (inherently sequential)
        int write_pos[half_len];
        int current_j = -1;
        for (i = 0; i < half_len; i++) {
            current_j++;
            write_pos[i] = current_j;
            if (mask[i]) {
                current_j++;
            }
        }

        // First write: vectorizable (scatter with computed indices)
        // Unrolled for better vectorization hints
        i = 0;
        for (; i <= half_len - 8; i += 8) {
            a[write_pos[i+0]] = b[i+0] + de[i+0];
            a[write_pos[i+1]] = b[i+1] + de[i+1];
            a[write_pos[i+2]] = b[i+2] + de[i+2];
            a[write_pos[i+3]] = b[i+3] + de[i+3];
            a[write_pos[i+4]] = b[i+4] + de[i+4];
            a[write_pos[i+5]] = b[i+5] + de[i+5];
            a[write_pos[i+6]] = b[i+6] + de[i+6];
            a[write_pos[i+7]] = b[i+7] + de[i+7];
        }
        for (; i < half_len; i++) {
            a[write_pos[i]] = b[i] + de[i];
        }

        // Second write: conditional scatter
        i = 0;
        for (; i <= half_len - 8; i += 8) {
            if (mask[i+0]) a[write_pos[i+0] + 1] = c[i+0] + de[i+0];
            if (mask[i+1]) a[write_pos[i+1] + 1] = c[i+1] + de[i+1];
            if (mask[i+2]) a[write_pos[i+2] + 1] = c[i+2] + de[i+2];
            if (mask[i+3]) a[write_pos[i+3] + 1] = c[i+3] + de[i+3];
            if (mask[i+4]) a[write_pos[i+4] + 1] = c[i+4] + de[i+4];
            if (mask[i+5]) a[write_pos[i+5] + 1] = c[i+5] + de[i+5];
            if (mask[i+6]) a[write_pos[i+6] + 1] = c[i+6] + de[i+6];
            if (mask[i+7]) a[write_pos[i+7] + 1] = c[i+7] + de[i+7];
        }
        for (; i < half_len; i++) {
            if (mask[i]) a[write_pos[i] + 1] = c[i] + de[i];
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s123(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s123(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
