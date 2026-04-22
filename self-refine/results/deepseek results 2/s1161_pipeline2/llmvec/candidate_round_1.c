#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute a[i] where c[i] >= 0
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (c[i] >= (float)0.) {
                a[i] = c[i] + d[i] * e[i];
            }
        }
        // Second loop: compute b[i] where c[i] < 0 using original a[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (c[i] < (float)0.) {
                b[i] = a[i] + d[i] * d[i];
            }
        }
    }
}

        #include <string.h>

void vectorized_s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        int n = LEN_1D - 1;
        int i = 0;

        // Vectorized first loop: a[i] = (c[i] >= 0) ? c[i] + d[i]*e[i] : a[i]
        for (; i <= n - 8; i += 8) {
            float c0 = c[i+0], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];
            float c4 = c[i+4], c5 = c[i+5], c6 = c[i+6], c7 = c[i+7];

            float val0 = c0 + d[i+0] * e[i+0];
            float val1 = c1 + d[i+1] * e[i+1];
            float val2 = c2 + d[i+2] * e[i+2];
            float val3 = c3 + d[i+3] * e[i+3];
            float val4 = c4 + d[i+4] * e[i+4];
            float val5 = c5 + d[i+5] * e[i+5];
            float val6 = c6 + d[i+6] * e[i+6];
            float val7 = c7 + d[i+7] * e[i+7];

            if (c0 >= 0.0f) a[i+0] = val0;
            if (c1 >= 0.0f) a[i+1] = val1;
            if (c2 >= 0.0f) a[i+2] = val2;
            if (c3 >= 0.0f) a[i+3] = val3;
            if (c4 >= 0.0f) a[i+4] = val4;
            if (c5 >= 0.0f) a[i+5] = val5;
            if (c6 >= 0.0f) a[i+6] = val6;
            if (c7 >= 0.0f) a[i+7] = val7;
        }
        for (; i < n; i++) {
            if (c[i] >= 0.0f) {
                a[i] = c[i] + d[i] * e[i];
            }
        }

        // Vectorized second loop: b[i] = (c[i] < 0) ? a[i] + d[i]*d[i] : b[i]
        i = 0;
        for (; i <= n - 8; i += 8) {
            float c0 = c[i+0], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];
            float c4 = c[i+4], c5 = c[i+5], c6 = c[i+6], c7 = c[i+7];

            float val0 = a[i+0] + d[i+0] * d[i+0];
            float val1 = a[i+1] + d[i+1] * d[i+1];
            float val2 = a[i+2] + d[i+2] * d[i+2];
            float val3 = a[i+3] + d[i+3] * d[i+3];
            float val4 = a[i+4] + d[i+4] * d[i+4];
            float val5 = a[i+5] + d[i+5] * d[i+5];
            float val6 = a[i+6] + d[i+6] * d[i+6];
            float val7 = a[i+7] + d[i+7] * d[i+7];

            if (c0 < 0.0f) b[i+0] = val0;
            if (c1 < 0.0f) b[i+1] = val1;
            if (c2 < 0.0f) b[i+2] = val2;
            if (c3 < 0.0f) b[i+3] = val3;
            if (c4 < 0.0f) b[i+4] = val4;
            if (c5 < 0.0f) b[i+5] = val5;
            if (c6 < 0.0f) b[i+6] = val6;
            if (c7 < 0.0f) b[i+7] = val7;
        }
        for (; i < n; i++) {
            if (c[i] < 0.0f) {
                b[i] = a[i] + d[i] * d[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s1161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s1161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
