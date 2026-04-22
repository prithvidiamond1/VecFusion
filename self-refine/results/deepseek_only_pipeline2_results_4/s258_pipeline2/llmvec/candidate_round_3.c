#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float aa0[256];
    for (int i = 0; i < 256; ++i) {
        aa0[i] = aa[0][i];
    }

    for (int nl = 0; nl < iterations; nl++) {
        float temp_s[256];
        float d_sq[256];

        // Precompute d[i]*d[i] for all i
        for (int i = 0; i < 256; ++i) {
            d_sq[i] = d[i] * d[i];
        }

        float s = 0.0f;
        // First loop: compute s values with dependency
        // Use arithmetic masking to eliminate branching
        for (int i = 0; i < 256; ++i) {
            float mask = (a[i] > 0.0f) ? 1.0f : 0.0f;
            s = mask * d_sq[i] + (1.0f - mask) * s;
            temp_s[i] = s;
        }

        // Second loop: compute b[i] and e[i] using precomputed s values
        for (int i = 0; i < 256; ++i) {
            float s_val = temp_s[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa0[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float aa0[256];
    for (int i = 0; i < 256; ++i) {
        aa0[i] = aa[0][i];
    }

    for (int nl = 0; nl < iterations; nl++) {
        float temp_s[256];
        float d_sq[256];

        // Vectorized precomputation of d_sq
        int i = 0;
        for (; i <= 252; i += 4) {
            d_sq[i] = d[i] * d[i];
            d_sq[i+1] = d[i+1] * d[i+1];
            d_sq[i+2] = d[i+2] * d[i+2];
            d_sq[i+3] = d[i+3] * d[i+3];
        }
        for (; i < 256; ++i) {
            d_sq[i] = d[i] * d[i];
        }

        // First loop: compute s values with dependency - keep scalar for correctness
        float s = 0.0f;
        for (i = 0; i < 256; ++i) {
            float mask = (a[i] > 0.0f) ? 1.0f : 0.0f;
            s = mask * d_sq[i] + (1.0f - mask) * s;
            temp_s[i] = s;
            a[i] = s;
        }

        // Second loop: vectorized computation of b[i] and e[i]
        i = 0;
        for (; i <= 252; i += 4) {
            float s_val0 = temp_s[i];
            float s_val1 = temp_s[i+1];
            float s_val2 = temp_s[i+2];
            float s_val3 = temp_s[i+3];

            b[i] = s_val0 * c[i] + d[i];
            b[i+1] = s_val1 * c[i+1] + d[i+1];
            b[i+2] = s_val2 * c[i+2] + d[i+2];
            b[i+3] = s_val3 * c[i+3] + d[i+3];

            e[i] = (s_val0 + 1.0f) * aa0[i];
            e[i+1] = (s_val1 + 1.0f) * aa0[i+1];
            e[i+2] = (s_val2 + 1.0f) * aa0[i+2];
            e[i+3] = (s_val3 + 1.0f) * aa0[i+3];
        }
        for (; i < 256; ++i) {
            float s_val = temp_s[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa0[i];
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
