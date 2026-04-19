#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    float s_array[256];
    float d_sq[256];

    // Precompute d[i]*d[i] once per outer iteration
    for (int i = 0; i < 256; ++i) {
        d_sq[i] = d[i] * d[i];
    }

    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;

        // First loop: compute s values using precomputed d_sq
        // This can now be vectorized with masked assignment
        for (int i = 0; i < 256; ++i) {
            // Branchless conditional update using comparison result as mask
            float mask = (a[i] > 0.0f) ? 1.0f : 0.0f;
            s = mask * d_sq[i] + (1.0f - mask) * s;
            s_array[i] = s;
        }

        // Second loop: compute b and e using stored s values
        // This loop is fully vectorizable
        for (int i = 0; i < 256; ++i) {
            float temp_s = s_array[i];
            b[i] = temp_s * c[i] + d[i];
            e[i] = (temp_s + 1.0f) * aa[0][i];
        }
    }
}

        #include <stddef.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    float s_array[256];
    float d_sq[256];

    // Precompute d[i]*d[i] once per outer iteration
    for (int i = 0; i < 256; i += 4) {
        float d0 = d[i];
        float d1 = d[i+1];
        float d2 = d[i+2];
        float d3 = d[i+3];
        d_sq[i] = d0 * d0;
        d_sq[i+1] = d1 * d1;
        d_sq[i+2] = d2 * d2;
        d_sq[i+3] = d3 * d3;
    }

    for (int nl = 0; nl < iterations; nl++) {
        s = 0.0f;

        // First loop: compute s values using precomputed d_sq
        for (int i = 0; i < 256; i += 4) {
            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];

            float mask0 = (a0 > 0.0f) ? 1.0f : 0.0f;
            float mask1 = (a1 > 0.0f) ? 1.0f : 0.0f;
            float mask2 = (a2 > 0.0f) ? 1.0f : 0.0f;
            float mask3 = (a3 > 0.0f) ? 1.0f : 0.0f;

            float dsq0 = d_sq[i];
            float dsq1 = d_sq[i+1];
            float dsq2 = d_sq[i+2];
            float dsq3 = d_sq[i+3];

            s = mask0 * dsq0 + (1.0f - mask0) * s;
            s_array[i] = s;
            s = mask1 * dsq1 + (1.0f - mask1) * s;
            s_array[i+1] = s;
            s = mask2 * dsq2 + (1.0f - mask2) * s;
            s_array[i+2] = s;
            s = mask3 * dsq3 + (1.0f - mask3) * s;
            s_array[i+3] = s;
        }

        // Second loop: compute b, e, and update a
        for (int i = 0; i < 256; i += 4) {
            float s0 = s_array[i];
            float s1 = s_array[i+1];
            float s2 = s_array[i+2];
            float s3 = s_array[i+3];

            float c0 = c[i];
            float c1 = c[i+1];
            float c2 = c[i+2];
            float c3 = c[i+3];

            float d0 = d[i];
            float d1 = d[i+1];
            float d2 = d[i+2];
            float d3 = d[i+3];

            float aa0 = aa[0][i];
            float aa1 = aa[0][i+1];
            float aa2 = aa[0][i+2];
            float aa3 = aa[0][i+3];

            // Compute b[i]
            float b0 = s0 * c0 + d0;
            float b1 = s1 * c1 + d1;
            float b2 = s2 * c2 + d2;
            float b3 = s3 * c3 + d3;
            b[i] = b0;
            b[i+1] = b1;
            b[i+2] = b2;
            b[i+3] = b3;

            // Compute e[i]
            float e0 = (s0 + 1.0f) * aa0;
            float e1 = (s1 + 1.0f) * aa1;
            float e2 = (s2 + 1.0f) * aa2;
            float e3 = (s3 + 1.0f) * aa3;
            e[i] = e0;
            e[i+1] = e1;
            e[i+2] = e2;
            e[i+3] = e3;

            // Update a[i] = b[i] + d[i] * e[i]
            a[i] = b0 + d0 * e0;
            a[i+1] = b1 + d1 * e1;
            a[i+2] = b2 + d2 * e2;
            a[i+3] = b3 + d3 * e3;
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128]; float aa_scalar[128]; float aa_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                s258(iterations, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar, aa_scalar); vectorized_s258(iterations, a_vector, b_vector, c_vector, d_vector, e_vector, aa_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
