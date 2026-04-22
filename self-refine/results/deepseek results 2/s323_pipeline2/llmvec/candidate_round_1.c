#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s323(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Precompute c*d and c*e into temporary arrays
        float cd[LEN_1D];
        float ce[LEN_1D];
        for (int i = 1; i < LEN_1D; i++) {
            cd[i] = c[i] * d[i];
            ce[i] = c[i] * e[i];
        }

        // Compute b[i] = b[i-1] + (cd[i] + ce[i]) using parallel prefix sum
        float b_temp[LEN_1D];
        b_temp[0] = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            b_temp[i] = cd[i] + ce[i];
        }

        // Serial prefix sum for recurrence (vectorizable with compiler support for reductions)
        for (int i = 1; i < LEN_1D; i++) {
            b_temp[i] = b_temp[i-1] + b_temp[i];
        }

        // Compute a[i] = b[i-1] + cd[i] (now vectorizable)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = b_temp[i-1] + cd[i];
        }

        // Copy b_temp back to b (vectorizable)
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b_temp[i];
        }
    }
}

        #include <string.h>

void vectorized_s323(int iterations, int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        float cd[LEN_1D];
        float ce[LEN_1D];

        // Vectorizable: compute cd and ce
        int i;
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            cd[i]   = c[i]   * d[i];
            cd[i+1] = c[i+1] * d[i+1];
            cd[i+2] = c[i+2] * d[i+2];
            cd[i+3] = c[i+3] * d[i+3];
            ce[i]   = c[i]   * e[i];
            ce[i+1] = c[i+1] * e[i+1];
            ce[i+2] = c[i+2] * e[i+2];
            ce[i+3] = c[i+3] * e[i+3];
        }
        for (; i < LEN_1D; i++) {
            cd[i] = c[i] * d[i];
            ce[i] = c[i] * e[i];
        }

        // Build b_temp with increments
        float b_temp[LEN_1D];
        b_temp[0] = b[0];

        // Vectorizable: compute increments
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            b_temp[i]   = cd[i]   + ce[i];
            b_temp[i+1] = cd[i+1] + ce[i+1];
            b_temp[i+2] = cd[i+2] + ce[i+2];
            b_temp[i+3] = cd[i+3] + ce[i+3];
        }
        for (; i < LEN_1D; i++) {
            b_temp[i] = cd[i] + ce[i];
        }

        // Serial prefix sum (recurrence, not vectorizable in general)
        for (i = 1; i < LEN_1D; i++) {
            b_temp[i] = b_temp[i-1] + b_temp[i];
        }

        // Vectorizable: compute a[i] = b_temp[i-1] + cd[i]
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            a[i]   = b_temp[i-1] + cd[i];
            a[i+1] = b_temp[i]   + cd[i+1];
            a[i+2] = b_temp[i+1] + cd[i+2];
            a[i+3] = b_temp[i+2] + cd[i+3];
        }
        for (; i < LEN_1D; i++) {
            a[i] = b_temp[i-1] + cd[i];
        }

        // Vectorizable: copy b_temp back to b
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            b[i]   = b_temp[i];
            b[i+1] = b_temp[i+1];
            b[i+2] = b_temp[i+2];
            b[i+3] = b_temp[i+3];
        }
        for (; i < LEN_1D; i++) {
            b[i] = b_temp[i];
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
                s323(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s323(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
