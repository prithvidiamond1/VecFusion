#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        // First pass: compute s values and store them
        float s_arr[256];
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.) {
                s = d[i] * d[i];
            }
            s_arr[i] = s;
        }
        // Second pass: compute b[i] - vectorizable
        for (int i = 0; i < 256; ++i) {
            b[i] = s_arr[i] * c[i] + d[i];
        }
        // Third pass: compute e[i] - vectorizable
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_arr[i] + (float)1.) * aa[0][i];
        }
    }
}

        #include <string.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.f;

        float s_arr[256];

        // First pass: compute s values (sequential due to dependency)
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.f) {
                s = d[i] * d[i];
            }
            s_arr[i] = s;
        }

        // Second pass: vectorizable
        int i = 0;
        for (; i <= 256 - 8; i += 8) {
            b[i+0] = s_arr[i+0] * c[i+0] + d[i+0];
            b[i+1] = s_arr[i+1] * c[i+1] + d[i+1];
            b[i+2] = s_arr[i+2] * c[i+2] + d[i+2];
            b[i+3] = s_arr[i+3] * c[i+3] + d[i+3];
            b[i+4] = s_arr[i+4] * c[i+4] + d[i+4];
            b[i+5] = s_arr[i+5] * c[i+5] + d[i+5];
            b[i+6] = s_arr[i+6] * c[i+6] + d[i+6];
            b[i+7] = s_arr[i+7] * c[i+7] + d[i+7];
        }
        for (; i < 256; ++i) {
            b[i] = s_arr[i] * c[i] + d[i];
        }

        // Third pass: vectorizable
        i = 0;
        for (; i <= 256 - 8; i += 8) {
            e[i+0] = (s_arr[i+0] + 1.f) * aa[0][i+0];
            e[i+1] = (s_arr[i+1] + 1.f) * aa[0][i+1];
            e[i+2] = (s_arr[i+2] + 1.f) * aa[0][i+2];
            e[i+3] = (s_arr[i+3] + 1.f) * aa[0][i+3];
            e[i+4] = (s_arr[i+4] + 1.f) * aa[0][i+4];
            e[i+5] = (s_arr[i+5] + 1.f) * aa[0][i+5];
            e[i+6] = (s_arr[i+6] + 1.f) * aa[0][i+6];
            e[i+7] = (s_arr[i+7] + 1.f) * aa[0][i+7];
        }
        for (; i < 256; ++i) {
            e[i] = (s_arr[i] + 1.f) * aa[0][i];
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128]; float aa_scalar[128]; float aa_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar)); fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                s258(iterations, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar, aa_scalar); vectorized_s258(iterations, a_vector, b_vector, c_vector, d_vector, e_vector, aa_vector);
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
} for (int i = 0; i < n; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
