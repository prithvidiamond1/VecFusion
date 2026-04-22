#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        // Loop distribution: process each position in the 5-element pattern separately
        // This creates uniform operations within each loop for better vectorization

        // Position 0: a[i] = a[i+1] * a[i]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i] = a[i + 1] * a[i];
        }

        // Position 1: a[i+1] = a[i+2] * a[i+1]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 1] = a[i + 2] * a[i + 1];
        }

        // Position 2: a[i+2] = a[i+3] * a[i+2]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 2] = a[i + 3] * a[i + 2];
        }

        // Position 3: a[i+3] = a[i+4] * a[i+3]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 3] = a[i + 4] * a[i + 3];
        }

        // Position 4: a[i+4] = a[i+5] * a[i+4]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 4] = a[i + 5] * a[i + 4];
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s116(int iterations, int LEN_1D, float* a) {
    int outer_iterations = iterations * 10;
    int limit = LEN_1D - 5;

    for (int nl = 0; nl < outer_iterations; nl++) {
        // Vectorized main loop: process groups of 4 pattern positions at once
        // Each iteration handles 4 complete 5-element patterns
        int vectorizable_len = ((limit - 0) / 4) * 4;

        for (int i = 0; i < vectorizable_len; i += 4) {
            // Load all needed elements for 4 complete patterns
            float4 pos0 = *(float4*)&a[i];
            float4 pos1 = *(float4*)&a[i + 1];
            float4 pos2 = *(float4*)&a[i + 2];
            float4 pos3 = *(float4*)&a[i + 3];
            float4 pos4 = *(float4*)&a[i + 4];
            float4 pos5 = *(float4*)&a[i + 5];

            // Perform all 5 updates in original order
            pos0 = pos0 * pos1;      // a[i] = a[i+1] * a[i]
            pos1 = pos1 * pos2;      // a[i+1] = a[i+2] * a[i+1]
            pos2 = pos2 * pos3;      // a[i+2] = a[i+3] * a[i+2]
            pos3 = pos3 * pos4;      // a[i+3] = a[i+4] * a[i+3]
            pos4 = pos4 * pos5;      // a[i+4] = a[i+5] * a[i+4]

            // Store results back
            *(float4*)&a[i] = pos0;
            *(float4*)&a[i + 1] = pos1;
            *(float4*)&a[i + 2] = pos2;
            *(float4*)&a[i + 3] = pos3;
            *(float4*)&a[i + 4] = pos4;
        }

        // Scalar cleanup: process remaining complete patterns one by one
        // This preserves the exact original semantics
        for (int i = vectorizable_len; i < limit; i++) {
            float t0 = a[i];
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];

            a[i] = t0 * t1;
            a[i + 1] = t1 * t2;
            a[i + 2] = t2 * t3;
            a[i + 3] = t3 * t4;
            a[i + 4] = t4 * t5;
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
