#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a,float* b,float* c, float* d,float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.) {
                s = d[i] * d[i];
            }
            b[i] = s * c[i] + d[i];
            e[i] = (s + (float)1.) * aa[0][i];
        }
    }
}

        #include <stdint.h>

typedef float float32_t;

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.0f;
        for (int i = 0; i < 256; i += 4) {
            // Load 4 elements
            float32_t a_vec[4] = {a[i], a[i+1], a[i+2], a[i+3]};
            float32_t d_vec[4] = {d[i], d[i+1], d[i+2], d[i+3]};
            float32_t c_vec[4] = {c[i], c[i+1], c[i+2], c[i+3]};
            float32_t aa0_vec[4] = {aa[0][i], aa[0][i+1], aa[0][i+2], aa[0][i+3]};

            // Compute s for each lane: if a > 0 then s = d*d else keep previous s
            float32_t s_vec[4];
            float32_t prev_s = s;
            for (int j = 0; j < 4; j++) {
                if (a_vec[j] > 0.0f) {
                    s_vec[j] = d_vec[j] * d_vec[j];
                } else {
                    s_vec[j] = (j == 0) ? prev_s : s_vec[j-1];
                }
            }

            // Compute b[i] = s * c[i] + d[i]
            float32_t b_vec[4];
            for (int j = 0; j < 4; j++) {
                b_vec[j] = s_vec[j] * c_vec[j] + d_vec[j];
            }
            b[i] = b_vec[0];
            b[i+1] = b_vec[1];
            b[i+2] = b_vec[2];
            b[i+3] = b_vec[3];

            // Compute e[i] = (s + 1.0) * aa[0][i]
            float32_t e_vec[4];
            for (int j = 0; j < 4; j++) {
                e_vec[j] = (s_vec[j] + 1.0f) * aa0_vec[j];
            }
            e[i] = e_vec[0];
            e[i+1] = e_vec[1];
            e[i+2] = e_vec[2];
            e[i+3] = e_vec[3];

            // Update s for next iteration (last lane's s)
            s = s_vec[3];
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
