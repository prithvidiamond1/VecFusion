#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s318(int iterations, int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // First loop: compute max absolute value (vectorizable reduction)
        float local_max = max;
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            local_max = (abs_val > local_max) ? abs_val : local_max;
            k += inc;
        }

        // Reset k for second pass
        k = inc;

        // Second loop: find first index where value equals max
        // Use temporary variable to store candidate index, then assign after loop
        int found_index = 0;
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            // Use comparison without branching
            int cond = (abs_val == local_max) && (found_index == 0);
            found_index = cond ? i : found_index;
            k += inc;
        }

        // Update final values
        max = local_max;
        index = found_index;
        chksum = max + (float) index;
    }
    return max + index + 1;
}

        #include <math.h>
#include <stdint.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // First loop: compute max absolute value
        // Unroll by 4 for vector-friendly code
        float local_max = max;

        // We can't easily vectorize with arbitrary inc stride,
        // but we can unroll to help the compiler
        int i = 1;
        int k0 = k;

        // Unrolled loop (4x unroll)
        float lm0 = local_max, lm1 = local_max, lm2 = local_max, lm3 = local_max;
        int limit = 1 + ((LEN_1D - 1) / 4) * 4;
        for (; i + 3 < LEN_1D; i += 4) {
            float av0 = fabsf(a[k0]);
            float av1 = fabsf(a[k0 + inc]);
            float av2 = fabsf(a[k0 + 2*inc]);
            float av3 = fabsf(a[k0 + 3*inc]);
            lm0 = (av0 > lm0) ? av0 : lm0;
            lm1 = (av1 > lm1) ? av1 : lm1;
            lm2 = (av2 > lm2) ? av2 : lm2;
            lm3 = (av3 > lm3) ? av3 : lm3;
            k0 += 4 * inc;
        }
        // Merge partial maxes
        lm0 = (lm1 > lm0) ? lm1 : lm0;
        lm2 = (lm3 > lm2) ? lm3 : lm2;
        local_max = (lm2 > lm0) ? lm2 : lm0;

        // Scalar tail
        for (; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k0]);
            local_max = (abs_val > local_max) ? abs_val : local_max;
            k0 += inc;
        }

        // Reset k for second pass
        k = inc;

        // Second loop: find first index where value equals max
        int found_index = 0;
        // Unrolled version - but must preserve first-found semantics
        // We'll do a simple unrolled loop collecting candidates
        i = 1;
        k0 = k;

        // Unroll by 4, collect minimum index where abs == local_max
        for (; i + 3 < LEN_1D; i += 4) {
            float av0 = fabsf(a[k0]);
            float av1 = fabsf(a[k0 + inc]);
            float av2 = fabsf(a[k0 + 2*inc]);
            float av3 = fabsf(a[k0 + 3*inc]);
            int c0 = (av0 == local_max) && (found_index == 0);
            found_index = c0 ? i : found_index;
            int c1 = (av1 == local_max) && (found_index == 0);
            found_index = c1 ? (i+1) : found_index;
            int c2 = (av2 == local_max) && (found_index == 0);
            found_index = c2 ? (i+2) : found_index;
            int c3 = (av3 == local_max) && (found_index == 0);
            found_index = c3 ? (i+3) : found_index;
            k0 += 4 * inc;
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k0]);
            int cond = (abs_val == local_max) && (found_index == 0);
            found_index = cond ? i : found_index;
            k0 += inc;
        }

        // Update final values
        max = local_max;
        index = found_index;
        chksum = max + (float) index;
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
