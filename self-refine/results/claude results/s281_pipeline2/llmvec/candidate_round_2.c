#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int half = LEN_1D / 2;
    for (int nl = 0; nl < iterations; nl++) {
        // Preload upper half of a into temp buffer to break the dependency for phase 1
        float temp1[half];
        for (int i = 0; i < half; i++) {
            temp1[i] = a[LEN_1D - i - 1];
        }

        // Phase 1: i in [0, half), reads from temp (original upper half values)
        for (int i = 0; i < half; i++) {
            float x = temp1[i] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }

        // Preload lower half of a (already updated in phase 1) into temp buffer
        // a[LEN_1D-i-1] for i in [half, LEN_1D) maps to a[0..half-1]
        float temp2[half];
        for (int i = 0; i < half; i++) {
            temp2[i] = a[i];
        }

        // Phase 2: i in [half, LEN_1D), reads from temp2 (no dependency on a[])
        for (int i = half; i < LEN_1D; i++) {
            float x = temp2[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }
}

        ```c
#include <stdint.h>

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int half = LEN_1D / 2;
    for (int nl = 0; nl < iterations; nl++) {
        float temp1[half];
        int i = 0;
        for (; i <= half - 8; i += 8) {
            temp1[i+0] = a[LEN_1D - (i+0) - 1];
            temp1[i+1] = a[LEN_1D - (i+1) - 1];
            temp1[i+2] = a[LEN_1D - (i+2) - 1];
            temp1[i+3] = a[LEN_1D - (i+3) - 1];
            temp1[i+4] = a[LEN_1D - (i+4) - 1];
            temp1[i+5] = a[LEN_1D - (i+5) - 1];
            temp1[i+6] = a[LEN_1D - (i+6) - 1];
            temp1[i+7] = a[LEN_1D - (i+7) - 1];
        }
        for (; i < half; i++) {
            temp1[i] = a[LEN_1D - i - 1];
        }

        i = 0;
        for (; i <= half - 8; i += 8) {
            float x0 = temp1[i+0] + b[i+0] * c[i+0];
            float x1 = temp1[i+1] + b[i+1] * c[i+1];
            float x2 = temp1[i+2] + b[i+2] * c[i+2];
            float x3 = temp1[i+3] + b[i+3] * c[i+3];
            float x4 = temp1[i+4] + b[i+4] * c[i+4];
            float x5 = temp1[i+5] + b[i+5] * c[i+5];
            float x6 = temp1[i+6] + b[i+6] * c[i+6];
            float x7 = temp1[i+7] + b[i+7] * c[i+7];
            a[i+0] = x0 - 1.0f; b[i+0] = x0;
            a[i+1] = x1 - 1.0f; b[i+1] = x1;
            a[i+2] = x2 - 1.0f; b[i+2] = x2;
            a[i+3] = x3 - 1.0f; b[i+3] = x3;
            a[i+4] = x4 - 1.0f; b[i+4] = x4;
            a[i+5] = x5 - 1.0f; b[i+5] = x5;
            a[i+6] = x6 - 1.0f; b[i+6] = x6;
            a[i+7] = x7 - 1.0f; b[i+7] = x7;
        }
        for (; i < half; i++) {
            float x = temp1[i] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }

        float temp2[half];
        i = 0;
        for (; i <= half - 8; i += 8) {
            temp2[i+0] = a[i+0];
            temp2[i+1] = a[i+1];
            temp2[i+2] = a[i+2];
            temp2[i+3] = a[i+3];
            temp2[i+4] = a[i+4];
            temp2[i+5] = a[i+5];
            temp2[i+6] = a[i+6];
            temp2[i+7] = a[i+7];
        }
        for (; i < half; i++) {
            temp2[i] = a[i];
        }

        i = half;
        for (; i <= LEN_1D - 8; i += 8) {
            float x0 = temp2[LEN_1D - (i+0) - 1] + b[i+0] * c[i+0];
            float x1 = temp2[LEN_1D - (i+1) - 1] + b[i+1] * c[i+1];
            float x2 = temp2[LEN_1D - (i+2) - 1] + b[i+2] * c[i+2];
            float x3 = temp2[LEN_1D - (i+3) - 1] + b[i+3] * c[i+3];
            float x4 = temp2[LEN_1D - (i+4) - 1] + b[i+4] * c[i+4];
            float x5 = temp2[LEN_1D - (i+5) - 1] + b[i+5] * c[i+5];
            float x6 = temp2[LEN_1D - (i+6) - 1] + b[i+6] * c[i+6];
            float x7 = temp2[LEN_1D - (i+7) - 1] + b[i+7] * c[i+7];
            a[i+0] = x0 - 1.0f; b[i+0] = x0;
            a[i+1] = x1 - 1.0f; b[i+1] = x1;
            a[i+2] = x2 - 1.0f; b[i+2] = x2;
            a[i+3] = x3 - 1.0f; b[i+3] = x3;
            a[i+4] = x4 - 1.0f; b[i+4] = x4;
            a[i+5] = x5 - 1.0f; b[i+5]

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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s281(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s281(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
