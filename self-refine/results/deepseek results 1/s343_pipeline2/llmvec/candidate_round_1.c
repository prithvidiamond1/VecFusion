#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s343(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int outer_loops = 10 * (iterations / 256);

    for (int nl = 0; nl < outer_loops; nl++) {
        int k = -1;

        // Process in column-major order (i outer, j inner)
        for (int i = 0; i < 256; i++) {
            // First, compute mask and prefix sum for this column
            int mask[256];
            int prefix_sum[256];
            int count = 0;

            // Vector-friendly condition check (no branching)
            for (int j = 0; j < 256; j++) {
                mask[j] = (bb[j][i] > 0.0f);
                count += mask[j];
                prefix_sum[j] = count;
            }

            // Then write the selected elements sequentially
            for (int j = 0; j < 256; j++) {
                if (mask[j]) {
                    int write_idx = k + prefix_sum[j];
                    flat_2d_array[write_idx] = aa[j][i];
                }
            }

            k += count;
        }
    }
}

        #include <string.h>

void vectorized_s343(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int outer_loops = 10 * (iterations / 256);

    for (int nl = 0; nl < outer_loops; nl++) {
        int k = -1;

        for (int i = 0; i < 256; i++) {
            int mask[256];
            int prefix_sum[256];
            int count = 0;

            // Unrolled mask computation (4-wide)
            int j = 0;
            for (; j <= 252; j += 4) {
                int m0 = (bb[j+0][i] > 0.0f);
                int m1 = (bb[j+1][i] > 0.0f);
                int m2 = (bb[j+2][i] > 0.0f);
                int m3 = (bb[j+3][i] > 0.0f);
                mask[j+0] = m0;
                mask[j+1] = m1;
                mask[j+2] = m2;
                mask[j+3] = m3;
                count += m0 + m1 + m2 + m3;
                prefix_sum[j+0] = count - (m1 + m2 + m3);
                prefix_sum[j+1] = count - (m2 + m3);
                prefix_sum[j+2] = count - m3;
                prefix_sum[j+3] = count;
            }
            // Scalar tail
            for (; j < 256; j++) {
                mask[j] = (bb[j][i] > 0.0f);
                count += mask[j];
                prefix_sum[j] = count;
            }

            // Write selected elements
            j = 0;
            for (; j <= 252; j += 4) {
                if (mask[j+0]) {
                    flat_2d_array[k + prefix_sum[j+0]] = aa[j+0][i];
                }
                if (mask[j+1]) {
                    flat_2d_array[k + prefix_sum[j+1]] = aa[j+1][i];
                }
                if (mask[j+2]) {
                    flat_2d_array[k + prefix_sum[j+2]] = aa[j+2][i];
                }
                if (mask[j+3]) {
                    flat_2d_array[k + prefix_sum[j+3]] = aa[j+3][i];
                }
            }
            for (; j < 256; j++) {
                if (mask[j]) {
                    flat_2d_array[k + prefix_sum[j]] = aa[j][i];
                }
            }

            k += count;
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
            int iterations = 5; float flat_2d_array_scalar[128]; float flat_2d_array_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(flat_2d_array_scalar, arr_len, &seed); memcpy(flat_2d_array_vector, flat_2d_array_scalar, sizeof(flat_2d_array_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s343(iterations, flat_2d_array_scalar, aa_scalar, bb_scalar); vectorized_s343(iterations, flat_2d_array_vector, aa_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((flat_2d_array_scalar[i]) - (flat_2d_array_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter flat_2d_array on trial %d at index %d\n", trial, i);
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
