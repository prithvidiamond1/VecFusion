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

        #include <stdint.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 200*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            k = (i+1) * ((i+1) - 1) / 2 + (i+1)-1;
            // The inner loop has a loop-carried dependency on k (k += j+1),
            // so we precompute the k values for each j, then do the accumulation.
            // k at iteration j = k_start + sum_{m=i}^{j-1} (m+1)
            // sum_{m=i}^{j-1} (m+1) = sum_{m=i+1}^{j} m = j*(j+1)/2 - i*(i+1)/2
            // So k(j) = k_start + j*(j+1)/2 - i*(i+1)/2
            // where k_start = (i+1)*i/2 + i = i*(i+1)/2 + i = i*(i+3)/2
            // Let base = k_start - i*(i+1)/2 = i*(i+3)/2 - i*(i+1)/2 = i*(i+3-i-1)/2 = i*2/2 = i
            // So k(j) = i + j*(j+1)/2
            // Let's verify: at j=i: k(i) = i + i*(i+1)/2
            // k_start = (i+1)*i/2 + i = i*(i+1)/2 + i. Yes, matches.

            // We can unroll the inner loop in chunks of 4
            int j = i;
            int limit = 256;

            // Scalar loop since k indices are non-contiguous (scatter pattern)
            // but we can unroll by 4 for better pipelining
            for (; j + 3 < limit; j += 4) {
                int k0 = i + j*(j+1)/2;
                int k1 = i + (j+1)*(j+2)/2;
                int k2 = i + (j+2)*(j+3)/2;
                int k3 = i + (j+3)*(j+4)/2;

                flat_2d_array[k0] += bb[j][i];
                flat_2d_array[k1] += bb[j+1][i];
                flat_2d_array[k2] += bb[j+2][i];
                flat_2d_array[k3] += bb[j+3][i];
            }
            // scalar tail
            for (; j < limit; j++) {
                int kj = i + j*(j+1)/2;
                flat_2d_array[kj] += bb[j][i];
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
