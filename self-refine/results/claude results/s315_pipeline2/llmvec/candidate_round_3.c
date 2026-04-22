#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s315(int iterations, int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x, chksum;
    int index;
    for (int nl = 0; nl < iterations; nl++) {
        /* First pass: find the maximum value using a pure float reduction (vectorizable) */
        float max_val = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            max_val = a[i] > max_val ? a[i] : max_val;
        }

        /* Second pass: find minimum index where a[i] == max_val
           Use negated-index max reduction: max of (-i) where a[i]==max_val
           Equivalent to finding the first (minimum) occurrence index.
           neg_idx starts at 0 (corresponds to index 0).
           For each i, if a[i]==max_val and -i > neg_idx, update neg_idx = -i.
           This is a standard max-reduction, vectorizable. */
        int neg_idx = 0; /* -index, starts at -(0) = 0 */
        for (int i = 0; i < LEN_1D; i++) {
            int c = (a[i] == max_val);
            int candidate = -i;
            neg_idx = (c & (candidate > neg_idx)) ? candidate : neg_idx;
        }

        x = max_val;
        index = -neg_idx;
        chksum = x + (float)index;
    }
    return index + x + 1;
}

        #include <float.h>

float vectorized_s315(int iterations, int LEN_1D, float* a)
{
    float x, chksum;
    int index;

    for (int nl = 0; nl < iterations; nl++) {
        /* First pass: find the maximum value using unrolled reduction */
        float max0 = -FLT_MAX, max1 = -FLT_MAX, max2 = -FLT_MAX, max3 = -FLT_MAX;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            float v0 = a[i+0];
            float v1 = a[i+1];
            float v2 = a[i+2];
            float v3 = a[i+3];
            max0 = v0 > max0 ? v0 : max0;
            max1 = v1 > max1 ? v1 : max1;
            max2 = v2 > max2 ? v2 : max2;
            max3 = v3 > max3 ? v3 : max3;
        }
        for (; i < LEN_1D; i++) {
            float v = a[i];
            max0 = v > max0 ? v : max0;
        }
        float tmp0 = max0 > max1 ? max0 : max1;
        float tmp1 = max2 > max3 ? max2 : max3;
        float max_val = tmp0 > tmp1 ? tmp0 : tmp1;

        /* Second pass: find minimum (first) index where a[i] == max_val */
        index = 0;
        for (int j = 0; j < LEN_1D; j++) {
            if (a[j] == max_val) { index = j; break; }
        }

        x = max_val;
        chksum = x + (float)index;
        (void)chksum;
    }
    return x + (float)index;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s315(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s315(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
