#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s_array[256];

    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;

        // First loop: compute all s values with dependency preserved
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.0f) {
                s = d[i] * d[i];
            }
            s_array[i] = s;
        }

        // Second loop: compute b and e using precomputed s values
        // This loop has no dependencies and can be vectorized
        for (int i = 0; i < 256; ++i) {
            float s_val = s_array[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa[0][i];
        }
    }
}

        #include <stddef.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s_array[256];

    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;

        /* First loop: sequential dependency on s */
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.0f) {
                s = d[i] * d[i];
            }
            s_array[i] = s;
        }

        /* Second loop: no cross-iteration dependency, vectorize with unrolling */
        int i = 0;
        for (; i <= 256 - 8; i += 8) {
            float s0 = s_array[i+0];
            float s1 = s_array[i+1];
            float s2 = s_array[i+2];
            float s3 = s_array[i+3];
            float s4 = s_array[i+4];
            float s5 = s_array[i+5];
            float s6 = s_array[i+6];
            float s7 = s_array[i+7];

            b[i+0] = s0 * c[i+0] + d[i+0];
            b[i+1] = s1 * c[i+1] + d[i+1];
            b[i+2] = s2 * c[i+2] + d[i+2];
            b[i+3] = s3 * c[i+3] + d[i+3];
            b[i+4] = s4 * c[i+4] + d[i+4];
            b[i+5] = s5 * c[i+5] + d[i+5];
            b[i+6] = s6 * c[i+6] + d[i+6];
            b[i+7] = s7 * c[i+7] + d[i+7];

            e[i+0] = (s0 + 1.0f) * aa[0][i+0];
            e[i+1] = (s1 + 1.0f) * aa[0][i+1];
            e[i+2] = (s2 + 1.0f) * aa[0][i+2];
            e[i+3] = (s3 + 1.0f) * aa[0][i+3];
            e[i+4] = (s4 + 1.0f) * aa[0][i+4];
            e[i+5] = (s5 + 1.0f) * aa[0][i+5];
            e[i+6] = (s6 + 1.0f) * aa[0][i+6];
            e[i+7] = (s7 + 1.0f) * aa[0][i+7];
        }
        for (; i < 256; ++i) {
            float sv = s_array[i];
            b[i] = sv * c[i] + d[i];
            e[i] = (sv + 1.0f) * aa[0][i];
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
