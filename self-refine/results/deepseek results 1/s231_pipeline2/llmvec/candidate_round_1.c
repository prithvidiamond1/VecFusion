#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s231(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 100 * (iterations / 256);
    if (loop_count <= 0) return;

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            float* aa_prev = aa[j - 1];
            float* aa_curr = aa[j];
            float* bb_curr = bb[j];

            for (int i = 0; i < 256; i++) {
                aa_curr[i] = aa_prev[i] + bb_curr[i];
            }
        }
    }
}

        #include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

void vectorized_s231(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 100 * (iterations / 256);
    if (loop_count <= 0) return;

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            float* aa_prev = aa[j - 1];
            float* aa_curr = aa[j];
            float* bb_curr = bb[j];

            int i = 0;
            for (; i <= 256 - 4; i += 4) {
                float4 vp, vc, vb, vr;
                __builtin_memcpy(&vp, aa_prev + i, sizeof(float4));
                __builtin_memcpy(&vb, bb_curr + i, sizeof(float4));
                vr = vp + vb;
                __builtin_memcpy(aa_curr + i, &vr, sizeof(float4));
            }
            for (; i < 256; i++) {
                aa_curr[i] = aa_prev[i] + bb_curr[i];
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s231(iterations, aa_scalar, bb_scalar); vectorized_s231(iterations, aa_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
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
