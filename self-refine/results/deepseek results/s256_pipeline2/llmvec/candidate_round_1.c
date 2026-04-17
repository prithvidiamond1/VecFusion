#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    int outer_iters = 10 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        // Store initial a[0] value
        float a0 = a[0];
        float a_temp[256];

        // Split: compute all a_temp values first (vectorizable pattern)
        for (int j = 1; j < 256; j++) {
            a_temp[j] = (j & 1) ? (1.0f - a0) : a0;
        }

        // Loop reordering: make j outer, i inner for stride-1 access
        for (int j = 1; j < 256; j++) {
            float aj = a_temp[j];
            float dj = d[j];
            // Vectorizable inner loop with stride-1 access
            for (int i = 0; i < 256; i++) {
                aa[j][i] = aj + bb[j][i] * dj;
            }
        }

        // Update original a array (separate loop)
        for (int j = 1; j < 256; j++) {
            a[j] = a_temp[j];
        }
    }
}

        #include <stdint.h>

typedef float v8sf __attribute__((vector_size(32)));

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    int outer_iters = 10 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        float a0 = a[0];
        float a_temp[256];

        // Compute a_temp values
        for (int j = 1; j < 256; j++) {
            a_temp[j] = (j & 1) ? (1.0f - a0) : a0;
        }

        // Vectorized inner loop using 8-wide float vectors
        for (int j = 1; j < 256; j++) {
            float aj = a_temp[j];
            float dj = d[j];

            v8sf vaj = {aj, aj, aj, aj, aj, aj, aj, aj};
            v8sf vdj = {dj, dj, dj, dj, dj, dj, dj, dj};

            float* aa_row = aa[j];
            float* bb_row = bb[j];

            int i = 0;
            for (; i <= 256 - 8; i += 8) {
                v8sf vbb;
                __builtin_memcpy(&vbb, bb_row + i, sizeof(v8sf));
                v8sf vres = vaj + vbb * vdj;
                __builtin_memcpy(aa_row + i, &vres, sizeof(v8sf));
            }
            // Scalar tail
            for (; i < 256; i++) {
                aa_row[i] = aj + bb_row[i] * dj;
            }
        }

        // Update a array
        for (int j = 1; j < 256; j++) {
            a[j] = a_temp[j];
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
