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

        // Precompute all absolute values into a temporary array
        // This allows vectorization of fabsf calls
        float* abs_vals = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        int current_k = k;
        for (int i = 1; i < LEN_1D; i++) {
            abs_vals[i] = fabsf(a[current_k]);
            current_k += inc;
        }

        // Separate reduction into two independent loops
        // First: find maximum value (simple reduction pattern)
        float max_val = max;
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = abs_vals[i];
            if (abs_val > max_val) {
                max_val = abs_val;
            }
        }

        // Second: find first index where value equals maximum
        // This avoids the complex argmax reduction pattern
        int max_idx = 0;
        for (int i = 1; i < LEN_1D; i++) {
            if (abs_vals[i] == max_val) {
                max_idx = i;
                break;  // Only need first occurrence
            }
        }

        // Update original variables
        index = max_idx;
        max = max_val;
        chksum = max + (float) index;
    }
    return max + index + 1;
}

        #include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    float max = 0.0f;
    int index = 0;

    for (int nl = 0; nl < iterations/2; nl++) {
        int k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        float max_val = max;
        int max_idx = 0;
        int current_k = k;

        // Process in chunks of 4
        int i;
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            // Load 4 values with correct stride pattern
            float v0 = fabsf(a[current_k]);
            current_k += inc;
            float v1 = fabsf(a[current_k]);
            current_k += inc;
            float v2 = fabsf(a[current_k]);
            current_k += inc;
            float v3 = fabsf(a[current_k]);
            current_k += inc;

            // Track maximum and correct index mapping
            if (v0 > max_val) { max_val = v0; max_idx = i; }
            if (v1 > max_val) { max_val = v1; max_idx = i + 1; }
            if (v2 > max_val) { max_val = v2; max_idx = i + 2; }
            if (v3 > max_val) { max_val = v3; max_idx = i + 3; }
        }

        // Scalar tail - process remaining elements
        for (; i < LEN_1D; i++) {
            float abs_val = fabsf(a[current_k]);
            if (abs_val > max_val) {
                max_val = abs_val;
                max_idx = i;
            }
            current_k += inc;
        }

        // Update original variables
        index = max_idx;
        max = max_val;
        float chksum = max + (float)index;
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
