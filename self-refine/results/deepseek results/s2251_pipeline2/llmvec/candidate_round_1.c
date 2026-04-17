#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;

        // Precompute all s values to break loop-carried dependency
        float* s_vals = (float*)alloca(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; i++) {
            s_vals[i] = b[i] + c[i];
        }

        // Fused computation of a[i] and b[i] to avoid false aliasing issues
        a[0] = s * e[0];
        b[0] = a[0] + d[0];

        for (int i = 1; i < LEN_1D; i++) {
            float ai = s_vals[i-1] * e[i];
            a[i] = ai;
            b[i] = ai + d[i];
        }
    }
}

        #include <alloca.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;

        // Precompute all s values
        float* s_vals = (float*)alloca(LEN_1D * sizeof(float));

        // Vectorized computation of s_vals[i] = b[i] + c[i]
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);
        for (; i < limit; i += 8) {
            s_vals[i+0] = b[i+0] + c[i+0];
            s_vals[i+1] = b[i+1] + c[i+1];
            s_vals[i+2] = b[i+2] + c[i+2];
            s_vals[i+3] = b[i+3] + c[i+3];
            s_vals[i+4] = b[i+4] + c[i+4];
            s_vals[i+5] = b[i+5] + c[i+5];
            s_vals[i+6] = b[i+6] + c[i+6];
            s_vals[i+7] = b[i+7] + c[i+7];
        }
        for (; i < LEN_1D; i++) {
            s_vals[i] = b[i] + c[i];
        }

        // First element uses s (which is 0.0)
        a[0] = s * e[0];
        b[0] = a[0] + d[0];

        // Vectorized loop for i = 1 to LEN_1D-1
        // a[i] = s_vals[i-1] * e[i]
        // b[i] = a[i] + d[i]
        i = 1;
        int limit2 = 1 + ((LEN_1D - 1) - ((LEN_1D - 1) % 8));
        for (; i < limit2; i += 8) {
            float ai0 = s_vals[i-1+0] * e[i+0];
            float ai1 = s_vals[i-1+1] * e[i+1];
            float ai2 = s_vals[i-1+2] * e[i+2];
            float ai3 = s_vals[i-1+3] * e[i+3];
            float ai4 = s_vals[i-1+4] * e[i+4];
            float ai5 = s_vals[i-1+5] * e[i+5];
            float ai6 = s_vals[i-1+6] * e[i+6];
            float ai7 = s_vals[i-1+7] * e[i+7];
            a[i+0] = ai0; b[i+0] = ai0 + d[i+0];
            a[i+1] = ai1; b[i+1] = ai1 + d[i+1];
            a[i+2] = ai2; b[i+2] = ai2 + d[i+2];
            a[i+3] = ai3; b[i+3] = ai3 + d[i+3];
            a[i+4] = ai4; b[i+4] = ai4 + d[i+4];
            a[i+5] = ai5; b[i+5] = ai5 + d[i+5];
            a[i+6] = ai6; b[i+6] = ai6 + d[i+6];
            a[i+7] = ai7; b[i+7] = ai7 + d[i+7];
        }
        for (; i < LEN_1D; i++) {
            float ai = s_vals[i-1] * e[i];
            a[i] = ai;
            b[i] = ai + d[i];
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
                s2251(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s2251(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
