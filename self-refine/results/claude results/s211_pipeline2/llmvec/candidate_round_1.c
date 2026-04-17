#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    float *b_orig = (float*)malloc(LEN_1D * sizeof(float));
    for (int nl = 0; nl < iterations; nl++) {
        memcpy(b_orig, b, LEN_1D * sizeof(float));
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b_orig[i + 1] - e[i] * d[i];
        }
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
        }
    }
    free(b_orig);
}

        #include <stdlib.h>
#include <string.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    float *b_orig = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations; nl++) {
        memcpy(b_orig, b, LEN_1D * sizeof(float));

        // First loop: b[i] = b_orig[i+1] - e[i] * d[i], for i in [1, LEN_1D-2]
        int i = 1;
        int end = LEN_1D - 1;
        int count = end - i;

        // Vectorize with width 8
        int vec_end = i + (count / 8) * 8;
        for (; i < vec_end; i += 8) {
            b[i+0] = b_orig[i+1] - e[i+0] * d[i+0];
            b[i+1] = b_orig[i+2] - e[i+1] * d[i+1];
            b[i+2] = b_orig[i+3] - e[i+2] * d[i+2];
            b[i+3] = b_orig[i+4] - e[i+3] * d[i+3];
            b[i+4] = b_orig[i+5] - e[i+4] * d[i+4];
            b[i+5] = b_orig[i+6] - e[i+5] * d[i+5];
            b[i+6] = b_orig[i+7] - e[i+6] * d[i+6];
            b[i+7] = b_orig[i+8] - e[i+7] * d[i+7];
        }
        // Scalar tail
        for (; i < end; i++) {
            b[i] = b_orig[i+1] - e[i] * d[i];
        }

        // Second loop: a[i] = b[i-1] + c[i] * d[i], for i in [1, LEN_1D-2]
        i = 1;
        count = end - i;
        vec_end = i + (count / 8) * 8;
        for (; i < vec_end; i += 8) {
            a[i+0] = b[i-1] + c[i+0] * d[i+0];
            a[i+1] = b[i+0] + c[i+1] * d[i+1];
            a[i+2] = b[i+1] + c[i+2] * d[i+2];
            a[i+3] = b[i+2] + c[i+3] * d[i+3];
            a[i+4] = b[i+3] + c[i+4] * d[i+4];
            a[i+5] = b[i+4] + c[i+5] * d[i+5];
            a[i+6] = b[i+5] + c[i+6] * d[i+6];
            a[i+7] = b[i+6] + c[i+7] * d[i+7];
        }
        // Scalar tail
        for (; i < end; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }

    free(b_orig);
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s211(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s211(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
