#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s321(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        float prev = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            float curr = a[i] + prev * b[i];
            prev = curr;
            a[i] = curr;
        }
    }
}

        void vectorized_s321(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        float prev = a[0];
        int i = 1;

        // Unrolled loop - still scalar due to loop-carried dependency on prev
        // but unrolling helps instruction-level parallelism and compiler optimization
        for (; i <= LEN_1D - 4; i += 4) {
            float c0 = a[i+0] + prev  * b[i+0];
            float c1 = a[i+1] + c0   * b[i+1];
            float c2 = a[i+2] + c1   * b[i+2];
            float c3 = a[i+3] + c2   * b[i+3];
            a[i+0] = c0;
            a[i+1] = c1;
            a[i+2] = c2;
            a[i+3] = c3;
            prev = c3;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float curr = a[i] + prev * b[i];
            prev = curr;
            a[i] = curr;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s321(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s321(iterations, LEN_1D, a_vector, b_vector);
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
