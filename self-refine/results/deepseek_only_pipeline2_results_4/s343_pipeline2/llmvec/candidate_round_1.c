#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s343(int iterations, float *restrict flat_2d_array, 
              float aa[256][256], float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = -1;

        for (int i = 0; i < 256; i++) {
            // Vector-friendly mask computation
            int mask[256];
            int count = 0;

            #pragma omp simd reduction(+:count)
            for (int j = 0; j < 256; j++) {
                mask[j] = (bb[j][i] > 0.0f);
                count += mask[j];
            }

            // Prefix sum - can be optimized with SIMD prefix sum
            int write_pos = k + 1;
            for (int j = 0; j < 256; j++) {
                if (mask[j]) {
                    flat_2d_array[write_pos] = aa[j][i];
                    write_pos++;
                }
            }
            k = write_pos - 1;
        }
    }
}

        #include <stddef.h>

void vectorized_s343(int iterations, float *restrict flat_2d_array,
                     float aa[256][256], float bb[256][256])
{
    int total_outer = 10 * (iterations / 256);

    for (int nl = 0; nl < total_outer; nl++) {
        int k = -1;

        for (int i = 0; i < 256; i++) {
            int mask[256];
            int count = 0;

            // Vectorized mask computation
            for (int j = 0; j < 256; j += 8) {
                // Process 8 elements at a time
                int mask0 = (bb[j][i] > 0.0f);
                int mask1 = (bb[j+1][i] > 0.0f);
                int mask2 = (bb[j+2][i] > 0.0f);
                int mask3 = (bb[j+3][i] > 0.0f);
                int mask4 = (bb[j+4][i] > 0.0f);
                int mask5 = (bb[j+5][i] > 0.0f);
                int mask6 = (bb[j+6][i] > 0.0f);
                int mask7 = (bb[j+7][i] > 0.0f);

                mask[j] = mask0;
                mask[j+1] = mask1;
                mask[j+2] = mask2;
                mask[j+3] = mask3;
                mask[j+4] = mask4;
                mask[j+5] = mask5;
                mask[j+6] = mask6;
                mask[j+7] = mask7;

                count += mask0 + mask1 + mask2 + mask3 + 
                         mask4 + mask5 + mask6 + mask7;
            }

            // Scalar prefix sum and write
            int write_pos = k + 1;
            for (int j = 0; j < 256; j++) {
                if (mask[j]) {
                    flat_2d_array[write_pos] = aa[j][i];
                    write_pos++;
                }
            }
            k = write_pos - 1;
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
