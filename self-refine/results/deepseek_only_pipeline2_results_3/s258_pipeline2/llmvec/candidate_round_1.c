#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float* aa_row = aa[0];

    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.f;
        float s_vals[256];

        // First loop: compute s values with sequential dependency
        // Use instruction reordering: compute condition first, then update s
        for (int i = 0; i < 256; ++i) {
            float condition = a[i] > 0.f ? 1.f : 0.f;
            // Store current s before potential update
            float current_s = s;
            // Update s based on condition
            s = condition * (d[i] * d[i]) + (1.f - condition) * current_s;
            s_vals[i] = s;
        }

        // Second loop: compute b[i] - fully vectorizable
        for (int i = 0; i < 256; ++i) {
            b[i] = s_vals[i] * c[i] + d[i];
        }

        // Third loop: compute e[i] - fully vectorizable  
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_vals[i] + 1.f) * aa_row[i];
        }
    }
}

        #include <stddef.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float* aa_row = aa[0];

    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.f;
        float s_vals[256];

        // First loop: compute s values with sequential dependency
        // Process in chunks of 4 for better locality
        for (int i = 0; i < 256; i += 4) {
            // Process 4 elements sequentially to maintain dependency
            for (int j = 0; j < 4 && i + j < 256; j++) {
                int idx = i + j;
                float condition = a[idx] > 0.f ? 1.f : 0.f;
                float current_s = s;
                s = condition * (d[idx] * d[idx]) + (1.f - condition) * current_s;
                s_vals[idx] = s;
            }
        }

        // Second loop: compute b[i] - vectorizable
        for (int i = 0; i < 256; i += 4) {
            int remaining = 256 - i;
            int limit = remaining < 4 ? remaining : 4;

            for (int j = 0; j < limit; j++) {
                int idx = i + j;
                b[idx] = s_vals[idx] * c[idx] + d[idx];
            }
        }

        // Third loop: compute e[i] - vectorizable
        for (int i = 0; i < 256; i += 4) {
            int remaining = 256 - i;
            int limit = remaining < 4 ? remaining : 4;

            for (int j = 0; j < limit; j++) {
                int idx = i + j;
                e[idx] = (s_vals[idx] + 1.f) * aa_row[idx];
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
