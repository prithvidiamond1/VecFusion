#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        // First loop: compute s values with dependency
        float s_vals[256];
        for (int i = 0; i < 256; ++i) {
            // Remove branching to help vectorization
            float condition = a[i] > 0.0f ? 1.0f : 0.0f;
            s = condition * (d[i] * d[i]) + (1.0f - condition) * s;
            s_vals[i] = s;
        }

        // Second loop: compute b[i] using stored s values
        for (int i = 0; i < 256; ++i) {
            b[i] = s_vals[i] * c[i] + d[i];
        }

        // Third loop: compute e[i] using stored s values
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_vals[i] + 1.0f) * aa[0][i];
        }
    }
}

        #include <stdint.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    typedef float v8f __attribute__((ext_vector_type(8)));

    const v8f vone = (v8f){1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        float s_vals[256];

        // First loop: scalar due to loop-carried dependency on s
        for (int i = 0; i < 256; i++) {
            s = (a[i] > 0.0f) ? (d[i] * d[i]) : s;
            s_vals[i] = s;
        }

        // Second loop: vectorizable - compute b[i] using stored s values
        int i = 0;
        for (; i <= 248; i += 8) {
            v8f s_vec = *((v8f*)&s_vals[i]);
            v8f c_vec = *((v8f*)&c[i]);
            v8f d_vec = *((v8f*)&d[i]);

            v8f b_vec = s_vec * c_vec + d_vec;
            *((v8f*)&b[i]) = b_vec;
        }

        // Scalar tail for second loop
        for (; i < 256; i++) {
            b[i] = s_vals[i] * c[i] + d[i];
        }

        // Third loop: vectorizable - compute e[i] using stored s values
        i = 0;
        for (; i <= 248; i += 8) {
            v8f s_vec = *((v8f*)&s_vals[i]);
            v8f aa_vec = *((v8f*)&aa[0][i]);

            v8f e_vec = (s_vec + vone) * aa_vec;
            *((v8f*)&e[i]) = e_vec;
        }

        // Scalar tail for third loop
        for (; i < 256; i++) {
            e[i] = (s_vals[i] + 1.0f) * aa[0][i];
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
