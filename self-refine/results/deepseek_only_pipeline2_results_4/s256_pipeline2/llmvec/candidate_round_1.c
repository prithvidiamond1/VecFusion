#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    for (int nl = 0; nl < 10 * (iterations / 256); nl++) {
        float a0 = a[0];

        for (int i = 0; i < 256; i++) {
            // Compute a[j] values for this i iteration using recurrence
            float a_prev = (i == 0) ? a0 : a[0];  // Start from appropriate value
            for (int j = 1; j < 256; j++) {
                a_prev = (float)1.0 - a_prev;
                a[j] = a_prev;
            }

            // Compute aa[j][i] using updated a[j] values
            for (int j = 1; j < 256; j++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    typedef float v8f __attribute__((ext_vector_type(8)));

    int outer_loop_count = 10 * (iterations / 256);

    for (int nl = 0; nl < outer_loop_count; nl++) {
        float a0 = a[0];

        for (int i = 0; i < 256; i++) {
            // Scalar recurrence for a[j] values
            float a_prev = (i == 0) ? a0 : a[0];
            for (int j = 1; j < 256; j++) {
                a_prev = 1.0f - a_prev;
                a[j] = a_prev;
            }

            // Vectorized computation of aa[j][i]
            int j = 1;
            for (; j + 7 < 256; j += 8) {
                v8f a_vec = {a[j], a[j+1], a[j+2], a[j+3], 
                            a[j+4], a[j+5], a[j+6], a[j+7]};
                v8f d_vec = {d[j], d[j+1], d[j+2], d[j+3], 
                            d[j+4], d[j+5], d[j+6], d[j+7]};
                v8f bb_vec = {bb[j][i], bb[j+1][i], bb[j+2][i], bb[j+3][i],
                             bb[j+4][i], bb[j+5][i], bb[j+6][i], bb[j+7][i]};

                v8f result = a_vec + bb_vec * d_vec;

                aa[j][i] = result[0];
                aa[j+1][i] = result[1];
                aa[j+2][i] = result[2];
                aa[j+3][i] = result[3];
                aa[j+4][i] = result[4];
                aa[j+5][i] = result[5];
                aa[j+6][i] = result[6];
                aa[j+7][i] = result[7];
            }

            // Scalar tail cleanup
            for (; j < 256; j++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float d_scalar[128]; float d_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s256(iterations, a_scalar, d_scalar, aa_scalar, bb_scalar); vectorized_s256(iterations, a_vector, d_vector, aa_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
