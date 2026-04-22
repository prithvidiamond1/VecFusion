#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = 1;
        // Process each column independently
        for (int i = 0; i < 256; i++) {
            // Compute all products for this column first
            float products[256];
            for (int j = 1; j < 256; j++) {
                products[j] = flat_2d_array[k-1] * cc[j][i];
                k++;
            }
            k++; // Extra increment after each column

            // Now update bb column with simple forward dependency
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j-1][i] + products[j];
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = 1;
        for (int i = 0; i < 256; i++) {
            float products[256];

            // Compute products for j=1..255
            // k starts at current value, we use flat_2d_array[k-1] for j=1..255
            // Unroll in groups of 8 for vectorization hint
            int base_k = k - 1; // index into flat_2d_array
            int j = 1;
            for (; j <= 252; j += 4) {
                products[j]   = flat_2d_array[base_k + (j-1)]   * cc[j][i];
                products[j+1] = flat_2d_array[base_k + (j)]     * cc[j+1][i];
                products[j+2] = flat_2d_array[base_k + (j+1)]   * cc[j+2][i];
                products[j+3] = flat_2d_array[base_k + (j+2)]   * cc[j+3][i];
            }
            for (; j < 256; j++) {
                products[j] = flat_2d_array[base_k + (j-1)] * cc[j][i];
            }

            k += 255; // advanced by 255 (j went from 1 to 255 inclusive)
            k++;      // extra increment after each column

            // Forward dependency scan - must be scalar
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j-1][i] + products[j];
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
            int iterations = 5; float bb_scalar[128]; float bb_vector[128]; float cc_scalar[128]; float cc_vector[128]; float flat_2d_array_scalar[128]; float flat_2d_array_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar)); fill_f32(cc_scalar, arr_len, &seed); memcpy(cc_vector, cc_scalar, sizeof(cc_scalar)); fill_f32(flat_2d_array_scalar, arr_len, &seed); memcpy(flat_2d_array_vector, flat_2d_array_scalar, sizeof(flat_2d_array_scalar));
                s126(iterations, bb_scalar, cc_scalar, flat_2d_array_scalar); vectorized_s126(iterations, bb_vector, cc_vector, flat_2d_array_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((cc_scalar[i]) - (cc_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter cc on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((flat_2d_array_scalar[i]) - (flat_2d_array_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter flat_2d_array on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
