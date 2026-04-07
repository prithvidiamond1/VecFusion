#include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        void saxpy_like(const int *a, const int *b, int *out, int n) {
    for (int i = 0; i < n; ++i) {
        out[i] = a[i] + 2 * b[i];
    }
}

        #include <stdint.h>

typedef int v8si __attribute__((vector_size(32)));

void vectorized_saxpy_like(const int *a, const int *b, int *out, int n) {
    int i = 0;
    int vec_len = 8; // 8 ints in 256-bit vector

    // Process 8 elements at a time using GCC/Clang vector extensions
    for (; i <= n - vec_len; i += vec_len) {
        v8si va, vb, vout;

        // Load 8 ints from a and b
        __builtin_memcpy(&va, a + i, sizeof(v8si));
        __builtin_memcpy(&vb, b + i, sizeof(v8si));

        // Compute a[i] + 2 * b[i]
        vout = va + 2 * vb;

        // Store result
        __builtin_memcpy(out + i, &vout, sizeof(v8si));
    }

    // Scalar cleanup for remaining elements
    for (; i < n; ++i) {
        out[i] = a[i] + 2 * b[i];
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

                saxpy_like(a, b, out_scalar, n);
                vectorized_saxpy_like(a, b, out_vector, n);

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
