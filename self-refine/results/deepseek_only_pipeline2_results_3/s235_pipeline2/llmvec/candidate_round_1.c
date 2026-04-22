#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // 1. Update all a[i] first (fully vectorizable)
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }

        // 2. Loop interchange: make j outer, i inner to enable vectorization across i
        // Process aa[0][i] as base for each column
        for (int j = 1; j < 256; j++) {
            // Vectorizable loop: no dependency across i
            for (int i = 0; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Vectorize a[i] += b[i] * c[i] with 8-wide SIMD
        typedef float v8f __attribute__((ext_vector_type(8)));
        int i = 0;
        for (; i + 7 < 256; i += 8) {
            v8f b_vec = *(v8f*)&b[i];
            v8f c_vec = *(v8f*)&c[i];
            v8f a_vec = *(v8f*)&a[i];
            a_vec += b_vec * c_vec;
            *(v8f*)&a[i] = a_vec;
        }
        // Scalar tail
        for (; i < 256; i++) {
            a[i] += b[i] * c[i];
        }

        // Process aa[j][i] = aa[j-1][i] + bb[j][i] * a[i]
        // Vectorize inner i-loop with 8-wide SIMD
        for (int j = 1; j < 256; j++) {
            i = 0;
            for (; i + 7 < 256; i += 8) {
                v8f aa_prev = *(v8f*)&aa[j-1][i];
                v8f bb_vec = *(v8f*)&bb[j][i];
                v8f a_vec = *(v8f*)&a[i];
                v8f aa_new = aa_prev + bb_vec * a_vec;
                *(v8f*)&aa[j][i] = aa_new;
            }
            // Scalar tail
            for (; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s235(iterations, a_scalar, b_scalar, c_scalar, aa_scalar, bb_scalar); vectorized_s235(iterations, a_vector, b_vector, c_vector, aa_vector, bb_vector);
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
