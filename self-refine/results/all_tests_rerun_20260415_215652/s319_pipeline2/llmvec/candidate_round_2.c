#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e)
{
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        sum = 0.;
        for (int i = 0; i < LEN_1D; i++) {
            float ai = c[i] + d[i];
            float bi = c[i] + e[i];
            a[i] = ai;
            b[i] = bi;
            sum += ai;
            sum += bi;
        }
    }
    return sum;
}

        #include <stdint.h>

float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e)
{
    float sum;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        sum = 0.0f;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);

        for (; i < limit; i += 8) {
            float ai0 = c[i+0] + d[i+0];
            float ai1 = c[i+1] + d[i+1];
            float ai2 = c[i+2] + d[i+2];
            float ai3 = c[i+3] + d[i+3];
            float ai4 = c[i+4] + d[i+4];
            float ai5 = c[i+5] + d[i+5];
            float ai6 = c[i+6] + d[i+6];
            float ai7 = c[i+7] + d[i+7];

            float bi0 = c[i+0] + e[i+0];
            float bi1 = c[i+1] + e[i+1];
            float bi2 = c[i+2] + e[i+2];
            float bi3 = c[i+3] + e[i+3];
            float bi4 = c[i+4] + e[i+4];
            float bi5 = c[i+5] + e[i+5];
            float bi6 = c[i+6] + e[i+6];
            float bi7 = c[i+7] + e[i+7];

            a[i+0] = ai0; a[i+1] = ai1; a[i+2] = ai2; a[i+3] = ai3;
            a[i+4] = ai4; a[i+5] = ai5; a[i+6] = ai6; a[i+7] = ai7;

            b[i+0] = bi0; b[i+1] = bi1; b[i+2] = bi2; b[i+3] = bi3;
            b[i+4] = bi4; b[i+5] = bi5; b[i+6] = bi6; b[i+7] = bi7;

            sum += ai0*bi0 + ai1*bi1 + ai2*bi2 + ai3*bi3
                 + ai4*bi4 + ai5*bi5 + ai6*bi6 + ai7*bi7;
        }

        for (; i < LEN_1D; i++) {
            float ai = c[i] + d[i];
            float bi = c[i] + e[i];
            a[i] = ai;
            b[i] = bi;
            sum += ai * bi;
        }
    }
    return sum;
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                ret_scalar = s319(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); ret_vector = vectorized_s319(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
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
