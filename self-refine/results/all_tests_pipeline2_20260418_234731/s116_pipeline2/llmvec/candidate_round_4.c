#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            // First, load all needed values into temporaries
            float t0 = a[i];
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];

            // Then compute all results
            float r0 = t1 * t0;
            float r1 = t2 * t1;
            float r2 = t3 * t2;
            float r3 = t4 * t3;
            float r4 = t5 * t4;

            // Finally, store all results
            a[i] = r0;
            a[i + 1] = r1;
            a[i + 2] = r2;
            a[i + 3] = r3;
            a[i + 4] = r4;
        }
    }
}

        void vectorized_s116(int iterations, int LEN_1D, float* a) {
    typedef float v4f __attribute__((vector_size(16)));
    const int vlen = 4;

    for (int nl = 0; nl < iterations * 10; nl++) {
        int i = 0;
        int limit = LEN_1D - 1;

        // Vectorized main loop: process 4 vectors (16 elements) per iteration
        // Ensure a[i + 3*vlen + 1] < LEN_1D for all reads
        for (; i + 3 * vlen + 1 < LEN_1D; i += vlen * 4) {
            // Load current vectors
            v4f v0 = *(v4f*)&a[i];
            v4f v1 = *(v4f*)&a[i + vlen];
            v4f v2 = *(v4f*)&a[i + 2 * vlen];
            v4f v3 = *(v4f*)&a[i + 3 * vlen];

            // Load next vectors (shifted by 1)
            v4f v0_next = *(v4f*)&a[i + 1];
            v4f v1_next = *(v4f*)&a[i + vlen + 1];
            v4f v2_next = *(v4f*)&a[i + 2 * vlen + 1];
            v4f v3_next = *(v4f*)&a[i + 3 * vlen + 1];

            // Compute results: a[i] = a[i+1] * a[i]
            v4f r0 = v0_next * v0;
            v4f r1 = v1_next * v1;
            v4f r2 = v2_next * v2;
            v4f r3 = v3_next * v3;

            // Store results
            *(v4f*)&a[i] = r0;
            *(v4f*)&a[i + vlen] = r1;
            *(v4f*)&a[i + 2 * vlen] = r2;
            *(v4f*)&a[i + 3 * vlen] = r3;
        }

        // Scalar cleanup for remaining elements
        // Process from i to limit inclusive (original loop condition: i < LEN_1D - 5)
        // But we need to handle up to limit = LEN_1D - 1 for the scalar operation
        for (; i <= limit; ++i) {
            a[i] = a[i + 1] * a[i];
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
