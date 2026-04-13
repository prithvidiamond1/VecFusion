#include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        void s1113(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = a[LEN_1D/2] + b[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s1113(float* a, float* b, float* out, int LEN_1D)
{
    float scalar_val = a[LEN_1D/2];
    int i = 0;
    int limit = LEN_1D - (LEN_1D % 8);
    for (; i < limit; i += 8) {
        out[i+0] = scalar_val + b[i+0];
        out[i+1] = scalar_val + b[i+1];
        out[i+2] = scalar_val + b[i+2];
        out[i+3] = scalar_val + b[i+3];
        out[i+4] = scalar_val + b[i+4];
        out[i+5] = scalar_val + b[i+5];
        out[i+6] = scalar_val + b[i+6];
        out[i+7] = scalar_val + b[i+7];
    }
    for (; i < LEN_1D; i++) {
        out[i] = scalar_val + b[i];
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

                s1113(a, b, out_scalar, n);
                vectorized_s1113(a, b, out_vector, n);

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
