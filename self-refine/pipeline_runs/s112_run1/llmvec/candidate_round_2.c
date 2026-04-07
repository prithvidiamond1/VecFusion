#include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        void s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        for (int i = LEN_1D - 2; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }
    }
}

        void vectorized_s112(int *a, int *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        int i = LEN_1D - 2;

        // Unroll by 8 - no loop-carried dependency since each iteration
        // writes a[i+1] and reads a[i], and i decreases, so no overlap
        for (; i >= 7; i -= 8) {
            a[i + 1] = a[i]     + b[i];
            a[i]     = a[i - 1] + b[i - 1];
            a[i - 1] = a[i - 2] + b[i - 2];
            a[i - 2] = a[i - 3] + b[i - 3];
            a[i - 3] = a[i - 4] + b[i - 4];
            a[i - 4] = a[i - 5] + b[i - 5];
            a[i - 5] = a[i - 6] + b[i - 6];
            a[i - 6] = a[i - 7] + b[i - 7];
        }

        // Scalar tail
        for (; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
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

        static uint64_t checksum_i32(const int *buf, int n) {
            uint64_t acc = 1469598103934665603ull;
            for (int i = 0; i < n; ++i) {
                acc ^= (uint32_t)buf[i];
                acc *= 1099511628211ull;
            }
            return acc;
        }

        int main(void) {
            const int n = 128;
            uint32_t seed = 7u;
            int a[128];
            int b[128];
            int out_scalar[128];
            int out_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_i32(a, n, &seed);
                fill_i32(b, n, &seed);
                memset(out_scalar, 0, sizeof(out_scalar));
                memset(out_vector, 0, sizeof(out_vector));

                s112(a, b, out_scalar, n);
                vectorized_s112(a, b, out_vector, n);

                if (memcmp(out_scalar, out_vector, sizeof(out_scalar)) != 0) {
                    fprintf(stderr, "Mismatch on trial %d\n", trial);
                    fprintf(stderr, "scalar_checksum=%llu\n", (unsigned long long)checksum_i32(out_scalar, n));
                    fprintf(stderr, "vector_checksum=%llu\n", (unsigned long long)checksum_i32(out_vector, n));
                    for (int i = 0; i < n; ++i) {
                        if (out_scalar[i] != out_vector[i]) {
                            fprintf(stderr, "first_diff_index=%d scalar=%d vector=%d\n", i, out_scalar[i], out_vector[i]);
                            break;
                        }
                    }
                    return 2;
                }
            }

            printf("PASS trials=%d checksum=%llu\n", 64, (unsigned long long)checksum_i32(out_vector, n));
            return 0;
        }
