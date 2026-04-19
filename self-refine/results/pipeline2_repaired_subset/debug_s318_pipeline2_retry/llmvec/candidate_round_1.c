#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s318(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;

    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // Split loop: first find max value (reduction)
        float local_max = max;
        int local_k = k;

        // Vectorizable reduction loop for max value only
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[local_k]);
            local_max = (abs_val > local_max) ? abs_val : local_max;
            local_k += inc;
        }

        // Second pass: find index of max value
        // This can run after max is known
        k = inc; // Reset k for index search
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            // Use comparison with pre-computed max
            if (abs_val == local_max) {
                index = i;
                break; // Found first occurrence
            }
            k += inc;
        }

        max = local_max;
        chksum = max + (float)index;
    }
    return max + index + 1;
}

        #include <math.h>
#include <stdint.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;

    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // Vector-friendly reduction for max value
        float local_max = max;
        int local_k = k;

        // Process in chunks of 4 for better vectorization
        int i;
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            float abs0 = fabsf(a[local_k]);
            float abs1 = fabsf(a[local_k + inc]);
            float abs2 = fabsf(a[local_k + 2*inc]);
            float abs3 = fabsf(a[local_k + 3*inc]);

            // Update local_max with all 4 values
            if (abs0 > local_max) local_max = abs0;
            if (abs1 > local_max) local_max = abs1;
            if (abs2 > local_max) local_max = abs2;
            if (abs3 > local_max) local_max = abs3;

            local_k += 4*inc;
        }

        // Handle remaining elements
        for (; i < LEN_1D; i++) {
            float abs_val = fabsf(a[local_k]);
            if (abs_val > local_max) local_max = abs_val;
            local_k += inc;
        }

        // Second pass: find index of max value
        k = inc;
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            if (abs_val == local_max) {
                index = i;
                break;
            }
            k += inc;
        }

        max = local_max;
        chksum = max + (float)index;
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
