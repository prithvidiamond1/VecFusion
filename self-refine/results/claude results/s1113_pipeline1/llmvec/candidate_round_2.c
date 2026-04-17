#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1113(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = a[LEN_1D/2] + b[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        int half = LEN_1D / 2;
        /* Process indices before LEN_1D/2 using the original a[half] value */
        float sv_before = a[half];
        int i = 0;
        /* Unrolled loop for i < half */
        int limit_before = half - (half % 8);
        for (; i < limit_before; i += 8) {
            a[i+0] = sv_before + b[i+0];
            a[i+1] = sv_before + b[i+1];
            a[i+2] = sv_before + b[i+2];
            a[i+3] = sv_before + b[i+3];
            a[i+4] = sv_before + b[i+4];
            a[i+5] = sv_before + b[i+5];
            a[i+6] = sv_before + b[i+6];
            a[i+7] = sv_before + b[i+7];
        }
        /* Scalar tail up to half */
        for (; i < half; i++) {
            a[i] = sv_before + b[i];
        }
        /* Write index half: a[half] = a[half] + b[half], updating a[half] */
        a[half] = sv_before + b[half];
        /* Now a[half] has been updated; use new value for i > half */
        float sv_after = a[half];
        i = half + 1;
        int limit_after = LEN_1D - ((LEN_1D - i) % 8);
        /* Align start for unrolling */
        int unroll_start = i;
        int unroll_limit = LEN_1D - ((LEN_1D - unroll_start) % 8);
        for (; i < unroll_limit; i += 8) {
            a[i+0] = sv_after + b[i+0];
            a[i+1] = sv_after + b[i+1];
            a[i+2] = sv_after + b[i+2];
            a[i+3] = sv_after + b[i+3];
            a[i+4] = sv_after + b[i+4];
            a[i+5] = sv_after + b[i+5];
            a[i+6] = sv_after + b[i+6];
            a[i+7] = sv_after + b[i+7];
        }
        for (; i < LEN_1D; i++) {
            a[i] = sv_after + b[i];
        }
        (void)limit_before;
        (void)limit_after;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s1113(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s1113(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
