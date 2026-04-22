#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 200*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            k = (i+1) * ((i+1) - 1) / 2 + (i+1)-1;
            for (int j = i; j < 256; j++) {
                flat_2d_array[k] += bb[j][i];
                k += j+1;
            }
        }
    }
}

        #include <stddef.h>

typedef float float_vec __attribute__((ext_vector_type(4)));

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int k;
    int nl_limit = 200 * (iterations / 256);
    for (int nl = 0; nl < nl_limit; nl++) {
        for (int i = 0; i < 256; i++) {
            k = (i+1) * ((i+1) - 1) / 2 + (i+1)-1;
            int j = i;
            // Vectorized main loop: process 4 elements at a time
            for (; j + 3 < 256; j += 4) {
                float_vec v_bb;
                v_bb[0] = bb[j][i];
                v_bb[1] = bb[j+1][i];
                v_bb[2] = bb[j+2][i];
                v_bb[3] = bb[j+3][i];
                float_vec v_arr;
                v_arr[0] = flat_2d_array[k];
                v_arr[1] = flat_2d_array[k + (j+1)];
                v_arr[2] = flat_2d_array[k + (j+1) + (j+2)];
                v_arr[3] = flat_2d_array[k + (j+1) + (j+2) + (j+3)];
                v_arr += v_bb;
                flat_2d_array[k] = v_arr[0];
                flat_2d_array[k + (j+1)] = v_arr[1];
                flat_2d_array[k + (j+1) + (j+2)] = v_arr[2];
                flat_2d_array[k + (j+1) + (j+2) + (j+3)] = v_arr[3];
                // Update k for next iteration
                k += (j+1) + (j+2) + (j+3) + (j+4);
            }
            // Scalar cleanup for remaining elements
            for (; j < 256; j++) {
                flat_2d_array[k] += bb[j][i];
                k += j+1;
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
            int iterations = 5; float flat_2d_array_scalar[128]; float flat_2d_array_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(flat_2d_array_scalar, arr_len, &seed); memcpy(flat_2d_array_vector, flat_2d_array_scalar, sizeof(flat_2d_array_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s141(iterations, flat_2d_array_scalar, bb_scalar); vectorized_s141(iterations, flat_2d_array_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((flat_2d_array_scalar[i]) - (flat_2d_array_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter flat_2d_array on trial %d at index %d\n", trial, i);
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
