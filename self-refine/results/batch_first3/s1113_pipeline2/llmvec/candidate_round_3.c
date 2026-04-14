#include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Hoist a[mid] into scalar before any writes to a[i]
        float amid = a[mid];
        // Phase 1: i = 0 to mid-1, use hoisted scalar, fully vectorizable
        for (int i = 0; i < mid; i++) {
            a[i] = amid + b[i];
        }
        // Mid-point update
        a[mid] = amid + b[mid];
        float tmp = a[mid];
        // Phase 2: i = mid+1 to LEN_1D-1, use updated scalar, fully vectorizable
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = tmp + b[i];
        }
    }
}

        #include <stdint.h>

typedef float v4sf __attribute__((vector_size(16)));

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float amid = a[mid];

        // Phase 1: i = 0 to mid-1
        {
            v4sf vamid = {amid, amid, amid, amid};
            int i = 0;
            int limit = mid - (mid % 4);
            for (; i < limit; i += 4) {
                v4sf vb = *((v4sf*)(b + i));
                v4sf res = vamid + vb;
                *((v4sf*)(a + i)) = res;
            }
            for (; i < mid; i++) {
                a[i] = amid + b[i];
            }
        }

        // Mid-point update
        a[mid] = amid + b[mid];
        float tmp = a[mid];

        // Phase 2: i = mid+1 to LEN_1D-1
        {
            v4sf vtmp = {tmp, tmp, tmp, tmp};
            int start = mid + 1;
            int len = LEN_1D - start;
            int i = start;
            int limit = start + (len - (len % 4));
            for (; i < limit; i += 4) {
                v4sf vb = *((v4sf*)(b + i));
                v4sf res = vtmp + vb;
                *((v4sf*)(a + i)) = res;
            }
            for (; i < LEN_1D; i++) {
                a[i] = tmp + b[i];
            }
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
