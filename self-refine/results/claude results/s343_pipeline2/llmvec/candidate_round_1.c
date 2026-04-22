#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s343(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int cond[256][256];
    int row_count[256];
    int row_start[257];

    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        // Compute conditions: cond[j][i] = bb[j][i] > 0, j-outer for contiguous bb access
        for (int j = 0; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                cond[j][i] = (bb[j][i] > (float)0.);
            }
        }

        // Compute per-row counts (vectorizable reduction)
        for (int j = 0; j < 256; j++) {
            int cnt = 0;
            for (int i = 0; i < 256; i++) {
                cnt += cond[j][i];
            }
            row_count[j] = cnt;
        }

        // Compute row start offsets (sequential prefix sum, only 256 iterations)
        row_start[0] = 0;
        for (int j = 0; j < 256; j++) {
            row_start[j+1] = row_start[j] + row_count[j];
        }

        // Scatter values using per-row local offset (each row independent)
        for (int j = 0; j < 256; j++) {
            int base = row_start[j];
            for (int i = 0; i < 256; i++) {
                flat_2d_array[base] = aa[j][i];
                base += cond[j][i];
            }
        }
    }
}

        #include <string.h>

void vectorized_s343(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int cond[256][256];
    int row_count[256];
    int row_start[257];

    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        // Compute conditions with unrolling for vectorization
        for (int j = 0; j < 256; j++) {
            int i = 0;
            for (; i <= 256 - 8; i += 8) {
                cond[j][i+0] = (bb[j][i+0] > 0.0f);
                cond[j][i+1] = (bb[j][i+1] > 0.0f);
                cond[j][i+2] = (bb[j][i+2] > 0.0f);
                cond[j][i+3] = (bb[j][i+3] > 0.0f);
                cond[j][i+4] = (bb[j][i+4] > 0.0f);
                cond[j][i+5] = (bb[j][i+5] > 0.0f);
                cond[j][i+6] = (bb[j][i+6] > 0.0f);
                cond[j][i+7] = (bb[j][i+7] > 0.0f);
            }
            for (; i < 256; i++) {
                cond[j][i] = (bb[j][i] > 0.0f);
            }
        }

        // Compute per-row counts with unrolling
        for (int j = 0; j < 256; j++) {
            int cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0;
            int cnt4 = 0, cnt5 = 0, cnt6 = 0, cnt7 = 0;
            int i = 0;
            for (; i <= 256 - 8; i += 8) {
                cnt0 += cond[j][i+0];
                cnt1 += cond[j][i+1];
                cnt2 += cond[j][i+2];
                cnt3 += cond[j][i+3];
                cnt4 += cond[j][i+4];
                cnt5 += cond[j][i+5];
                cnt6 += cond[j][i+6];
                cnt7 += cond[j][i+7];
            }
            int cnt = (cnt0 + cnt1) + (cnt2 + cnt3) + (cnt4 + cnt5) + (cnt6 + cnt7);
            for (; i < 256; i++) {
                cnt += cond[j][i];
            }
            row_count[j] = cnt;
        }

        // Prefix sum (sequential)
        row_start[0] = 0;
        for (int j = 0; j < 256; j++) {
            row_start[j+1] = row_start[j] + row_count[j];
        }

        // Scatter values
        for (int j = 0; j < 256; j++) {
            int base = row_start[j];
            for (int i = 0; i < 256; i++) {
                flat_2d_array[base] = aa[j][i];
                base += cond[j][i];
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; float flat_2d_array_scalar[128]; float flat_2d_array_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(flat_2d_array_scalar, n, &seed); memcpy(flat_2d_array_vector, flat_2d_array_scalar, sizeof(flat_2d_array_scalar)); fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, n, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s343(iterations, flat_2d_array_scalar, aa_scalar, bb_scalar); vectorized_s343(iterations, flat_2d_array_vector, aa_vector, bb_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((flat_2d_array_scalar[i]) - (flat_2d_array_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter flat_2d_array on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
