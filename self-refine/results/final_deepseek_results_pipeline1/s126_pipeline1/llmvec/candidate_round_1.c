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
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j-1][i] + flat_2d_array[k-1] * cc[j][i];
                ++k;
            }
            ++k;
        }
    }
}

        #include <stdint.h>

typedef float float32x4_t __attribute__((__vector_size__(16)));

void vectorized_s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int total_iters = 10 * (iterations / 256);
    for (int nl = 0; nl < total_iters; nl++) {
        int k = 1;
        for (int i = 0; i < 256; i++) {
            int j = 1;
            // Vectorized main loop: process 4 columns at a time
            for (; j + 4 <= 256; j += 4) {
                float32x4_t bb_prev = { bb[j-1][i], bb[j][i], bb[j+1][i], bb[j+2][i] };
                float32x4_t cc_vals = { cc[j][i], cc[j+1][i], cc[j+2][i], cc[j+3][i] };
                float32x4_t flat_vals = { flat_2d_array[k-1], flat_2d_array[k], flat_2d_array[k+1], flat_2d_array[k+2] };
                float32x4_t result = bb_prev + flat_vals * cc_vals;
                bb[j][i] = result[0];
                bb[j+1][i] = result[1];
                bb[j+2][i] = result[2];
                bb[j+3][i] = result[3];
                k += 4;
            }
            // Scalar cleanup for remaining columns
            for (; j < 256; j++) {
                bb[j][i] = bb[j-1][i] + flat_2d_array[k-1] * cc[j][i];
                ++k;
            }
            ++k; // skip the last element of the row
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
