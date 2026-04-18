#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s292(int iterations,int LEN_1D, float* a,float* b)
{
    int im1, im2;
    for (int nl = 0; nl < iterations; nl++) {
        im1 = LEN_1D-1;
        im2 = LEN_1D-2;
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = (b[i] + b[im1] + b[im2]) * (float).333;
            im2 = im1;
            im1 = i;
        }
    }
}

        void vectorized_s292(int iterations, int LEN_1D, float* a, float* b)
{
    int im1, im2;
    for (int nl = 0; nl < iterations; nl++) {
        im1 = LEN_1D - 1;
        im2 = LEN_1D - 2;
        // The first two iterations have dependencies on im1/im2 that wrap around,
        // handle them scalar first, then vectorize the rest.
        // Actually, let's just unroll manually in groups of 4 where possible.
        // For i=0: im1=LEN_1D-1, im2=LEN_1D-2
        // For i=1: im1=0,        im2=LEN_1D-1
        // For i=2: im1=1,        im2=0
        // For i=k (k>=2): im1=k-1, im2=k-2
        // So for i>=2, a[i] = (b[i] + b[i-1] + b[i-2]) * 0.333f
        // Handle i=0 and i=1 scalar:
        if (LEN_1D > 0) {
            a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * 0.333f;
        }
        if (LEN_1D > 1) {
            a[1] = (b[1] + b[0] + b[LEN_1D-1]) * 0.333f;
        }
        // Vectorize i=2..LEN_1D-1
        int i = 2;
        int limit = LEN_1D - ((LEN_1D - 2) % 4);
        for (; i < limit; i += 4) {
            a[i]   = (b[i]   + b[i-1] + b[i-2]) * 0.333f;
            a[i+1] = (b[i+1] + b[i]   + b[i-1]) * 0.333f;
            a[i+2] = (b[i+2] + b[i+1] + b[i])   * 0.333f;
            a[i+3] = (b[i+3] + b[i+2] + b[i+1]) * 0.333f;
        }
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * 0.333f;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s292(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s292(iterations, LEN_1D, a_vector, b_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
