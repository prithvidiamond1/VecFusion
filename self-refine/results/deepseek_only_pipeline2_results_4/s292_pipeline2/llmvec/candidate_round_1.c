#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s292(int iterations,int LEN_1D, float* a,float* b)
{
    int im1, im2;
    float prev2, prev1, current;

    for (int nl = 0; nl < iterations; nl++) {
        // Handle first two elements separately (special cases)
        if (LEN_1D > 0) {
            // i = 0: b[LEN_1D-1], b[LEN_1D-2]
            a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float)0.333;
        }

        if (LEN_1D > 1) {
            // i = 1: b[1], b[LEN_1D-1], b[0]
            a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float)0.333;
        }

        // Main loop: i from 2 to LEN_1D-1
        // Now we have regular pattern: b[i], b[i-1], b[i-2]
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float)0.333;
        }
    }
}

        #include <stddef.h>

void vectorized_s292(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        // Handle first two elements separately (special cases)
        if (LEN_1D > 0) {
            a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float)0.333;
        }

        if (LEN_1D > 1) {
            a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float)0.333;
        }

        // Vectorized main loop
        int i = 2;
        for (; i + 3 < LEN_1D; i += 4) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float)0.333;
            a[i+1] = (b[i+1] + b[i] + b[i-1]) * (float)0.333;
            a[i+2] = (b[i+2] + b[i+1] + b[i]) * (float)0.333;
            a[i+3] = (b[i+3] + b[i+2] + b[i+1]) * (float)0.333;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float)0.333;
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
