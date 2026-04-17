#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            // Load all original values before any updates
            float t0 = a[i];
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];

            // Perform all computations using original values
            a[i] = t1 * t0;
            a[i + 1] = t2 * t1;
            a[i + 2] = t3 * t2;
            a[i + 3] = t4 * t3;
            a[i + 4] = t5 * t4;
        }
    }
}

        #include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

void vectorized_s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        // The inner loop steps by 5, processing a[i..i+4] using a[i..i+5]
        // Each iteration: a[i+k] = a[i+k+1] * a[i+k] for k=0..4
        // We can vectorize by loading two overlapping windows of 5 elements
        // and multiplying them element-wise.

        int limit = LEN_1D - 5;
        int i = 0;

        // Process 4 iterations of the outer step-5 loop at a time (20 elements)
        // Each group of 5: need a[i..i+5], next group: a[i+5..i+10], etc.
        // For 4 groups: we need a[i..i+20+1] = a[i..i+21]
        // result[i+k] = a[i+k+1] * a[i+k] for k in 0..19 (but stepping by 5)

        // Simpler approach: unroll 2 iterations of the step-5 loop
        // Each unrolled step handles 10 elements, needs a[i..i+10]

        for (i = 0; i + 10 <= limit; i += 10) {
            // Load 11 floats: a[i..i+10]
            float t0  = a[i];
            float t1  = a[i+1];
            float t2  = a[i+2];
            float t3  = a[i+3];
            float t4  = a[i+4];
            float t5  = a[i+5];
            float t6  = a[i+6];
            float t7  = a[i+7];
            float t8  = a[i+8];
            float t9  = a[i+9];
            float t10 = a[i+10];

            // First group (i..i+4)
            a[i]   = t1  * t0;
            a[i+1] = t2  * t1;
            a[i+2] = t3  * t2;
            a[i+3] = t4  * t3;
            a[i+4] = t5  * t4;

            // Second group (i+5..i+9)
            a[i+5] = t6  * t5;
            a[i+6] = t7  * t6;
            a[i+7] = t8  * t7;
            a[i+8] = t9  * t8;
            a[i+9] = t10 * t9;
        }

        // Scalar cleanup for remaining step-5 iterations
        for (; i < limit; i += 5) {
            float t0 = a[i];
            float t1 = a[i+1];
            float t2 = a[i+2];
            float t3 = a[i+3];
            float t4 = a[i+4];
            float t5 = a[i+5];

            a[i]   = t1 * t0;
            a[i+1] = t2 * t1;
            a[i+2] = t3 * t2;
            a[i+3] = t4 * t3;
            a[i+4] = t5 * t4;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                s116(iterations, LEN_1D, a_scalar); vectorized_s116(iterations, LEN_1D, a_vector);
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
