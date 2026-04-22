#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int n = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // Precompute d[i]*e[i] for all i (vectorizable)
        float de[n];
        for (int i = 0; i < n; i++) {
            de[i] = d[i] * e[i];
        }

        // Compute b[i] + de[i] for all i (vectorizable)
        float b_plus_de[n];
        for (int i = 0; i < n; i++) {
            b_plus_de[i] = b[i] + de[i];
        }

        // Compute c[i] + de[i] and condition mask (vectorizable)
        float c_plus_de[n];
        int mask[n];
        for (int i = 0; i < n; i++) {
            mask[i] = (c[i] > 0.0f);
            c_plus_de[i] = c[i] + de[i];
        }

        // Precompute write indices using prefix sum (vectorizable)
        int indices[n];
        indices[0] = 0;
        for (int i = 1; i < n; i++) {
            indices[i] = indices[i-1] + 1 + mask[i-1];
        }

        // First write pass: always write b_plus_de (vectorizable)
        for (int i = 0; i < n; i++) {
            a[indices[i]] = b_plus_de[i];
        }

        // Second write pass: conditional writes for c_plus_de (vectorizable)
        for (int i = 0; i < n; i++) {
            if (mask[i]) {
                a[indices[i] + 1] = c_plus_de[i];
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int n = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // Precompute de[i] = d[i] * e[i], unrolled 4x
        float de[n];
        int i = 0;
        for (; i <= n - 4; i += 4) {
            de[i+0] = d[i+0] * e[i+0];
            de[i+1] = d[i+1] * e[i+1];
            de[i+2] = d[i+2] * e[i+2];
            de[i+3] = d[i+3] * e[i+3];
        }
        for (; i < n; i++) {
            de[i] = d[i] * e[i];
        }

        // Compute b_plus_de[i] = b[i] + de[i], unrolled 4x
        float b_plus_de[n];
        i = 0;
        for (; i <= n - 4; i += 4) {
            b_plus_de[i+0] = b[i+0] + de[i+0];
            b_plus_de[i+1] = b[i+1] + de[i+1];
            b_plus_de[i+2] = b[i+2] + de[i+2];
            b_plus_de[i+3] = b[i+3] + de[i+3];
        }
        for (; i < n; i++) {
            b_plus_de[i] = b[i] + de[i];
        }

        // Compute c_plus_de[i] and mask[i], unrolled 4x
        float c_plus_de[n];
        int mask[n];
        i = 0;
        for (; i <= n - 4; i += 4) {
            mask[i+0] = (c[i+0] > 0.0f) ? 1 : 0;
            mask[i+1] = (c[i+1] > 0.0f) ? 1 : 0;
            mask[i+2] = (c[i+2] > 0.0f) ? 1 : 0;
            mask[i+3] = (c[i+3] > 0.0f) ? 1 : 0;
            c_plus_de[i+0] = c[i+0] + de[i+0];
            c_plus_de[i+1] = c[i+1] + de[i+1];
            c_plus_de[i+2] = c[i+2] + de[i+2];
            c_plus_de[i+3] = c[i+3] + de[i+3];
        }
        for (; i < n; i++) {
            mask[i] = (c[i] > 0.0f) ? 1 : 0;
            c_plus_de[i] = c[i] + de[i];
        }

        // Prefix sum for indices (sequential due to dependency)
        int indices[n];
        if (n > 0) {
            indices[0] = 0;
            for (i = 1; i < n; i++) {
                indices[i] = indices[i-1] + 1 + mask[i-1];
            }
        }

        // First write pass: always write b_plus_de, unrolled 4x
        i = 0;
        for (; i <= n - 4; i += 4) {
            a[indices[i+0]] = b_plus_de[i+0];
            a[indices[i+1]] = b_plus_de[i+1];
            a[indices[i+2]] = b_plus_de[i+2];
            a[indices[i+3]] = b_plus_de[i+3];
        }
        for (; i < n; i++) {
            a[indices[i]] = b_plus_de[i];
        }

        // Second write pass: conditional writes for c_plus_de, unrolled 4x
        i = 0;
        for (; i <= n - 4; i += 4) {
            if (mask[i+0]) a[indices[i+0] + 1] = c_plus_de[i+0];
            if (mask[i+1]) a[indices[i+1] + 1] = c_plus_de[i+1];
            if (mask[i+2]) a[indices[i+2] + 1] = c_plus_de[i+2];
            if (mask[i+3]) a[indices[i+3] + 1] = c_plus_de[i+3];
        }
        for (; i < n; i++) {
            if (mask[i]) a[indices[i] + 1] = c_plus_de[i];
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
