#include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        bool AllPointsEq_opt(const int pts[], int count) {
    int result = 1;
    for (int i = 1; i < count; ++i) {
        result &= (pts[0] == pts[i]);
    }
    return (bool)result;
}

        #include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef int v8si __attribute__((vector_size(32)));

bool vectorized_AllPointsEq(const int a[], const int b[], bool out_vector[], int n) {
    int i = 0;

    if (n >= 8) {
        v8si ones = {1, 1, 1, 1, 1, 1, 1, 1};

        for (; i + 7 < n; i += 8) {
            v8si va, vb;
            __builtin_memcpy(&va, &a[i], sizeof(v8si));
            __builtin_memcpy(&vb, &b[i], sizeof(v8si));
            v8si cmp = (va == vb);
            v8si mask = cmp & ones;
            out_vector[i+0] = (bool)mask[0];
            out_vector[i+1] = (bool)mask[1];
            out_vector[i+2] = (bool)mask[2];
            out_vector[i+3] = (bool)mask[3];
            out_vector[i+4] = (bool)mask[4];
            out_vector[i+5] = (bool)mask[5];
            out_vector[i+6] = (bool)mask[6];
            out_vector[i+7] = (bool)mask[7];
        }
    }

    for (; i < n; ++i) {
        out_vector[i] = (bool)(a[i] == b[i]);
    }

    bool result = true;
    for (int j = 0; j < n; ++j) {
        result &= out_vector[j];
    }
    return result;
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

                AllPointsEq(a, b, out_scalar, n);
                vectorized_AllPointsEq(a, b, out_vector, n);

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
