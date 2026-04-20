#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s318(int iterations,int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // Loop distribution: separate absolute value computation from max/index search
        float current_max = max;
        int current_index = 0;

        // Vector-friendly loop without branching
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            int update = (abs_val > current_max);
            current_max = update ? abs_val : current_max;
            current_index = update ? i : current_index;
            k += inc;
        }

        index = current_index;
        max = current_max;

        chksum = max + (float) index;
    }
    return max + index + 1;
}

        #include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    int index;
    float max;

    for (int nl = 0; nl < iterations/2; nl++) {
        max = fabsf(a[0]);
        index = 0;

        float current_max = max;
        int current_index = 0;

        // Vectorization-friendly loop with explicit stride calculation
        int i = 1;
        int loop_bound = LEN_1D;

        // Process elements in chunks
        for (; i + 3 < loop_bound; i += 4) {
            // Compute indices with stride
            int k0 = i * inc;
            int k1 = (i + 1) * inc;
            int k2 = (i + 2) * inc;
            int k3 = (i + 3) * inc;

            // Load and compute absolute values
            float abs_val0 = fabsf(a[k0]);
            float abs_val1 = fabsf(a[k1]);
            float abs_val2 = fabsf(a[k2]);
            float abs_val3 = fabsf(a[k3]);

            // Check and update for each element
            if (abs_val0 > current_max) {
                current_max = abs_val0;
                current_index = i;
            }
            if (abs_val1 > current_max) {
                current_max = abs_val1;
                current_index = i + 1;
            }
            if (abs_val2 > current_max) {
                current_max = abs_val2;
                current_index = i + 2;
            }
            if (abs_val3 > current_max) {
                current_max = abs_val3;
                current_index = i + 3;
            }
        }

        // Scalar tail cleanup with correct stride
        for (; i < loop_bound; i++) {
            int k = i * inc;
            float abs_val = fabsf(a[k]);
            if (abs_val > current_max) {
                current_max = abs_val;
                current_index = i;
            }
        }

        index = current_index;
        max = current_max;

        // chksum computation (unused but preserved)
        float chksum = max + (float) index;
    }
    return max + index + 1;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; int inc = 7; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s318(iterations, LEN_1D, a_scalar, inc); ret_vector = vectorized_s318(iterations, LEN_1D, a_vector, inc); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
