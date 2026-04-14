#include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        bool AllPointsEq_opt(const int pts[], int count) {
    int first = pts[0];
    int result = 1;
    for (int i = 1; i < count; ++i) {
        result &= (first == pts[i]);
    }
    return (bool)result;
}

        #include <stdint.h>
#include <string.h>

int vectorized_AllPointsEq(const int* a, const int* b, int* out, int n) {
    (void)b;
    if (n <= 0) { if (out) *out = 1; return 1; }

    int first = a[0];

    typedef int v8si __attribute__((vector_size(32)));

    int i = 1;
    int result = 1;

    if ((n - 1) >= 8) {
        v8si vfirst = {first, first, first, first, first, first, first, first};
        v8si vresult = {1, 1, 1, 1, 1, 1, 1, 1};

        for (; i + 7 < n; i += 8) {
            v8si vdata;
            memcpy(&vdata, &a[i], sizeof(v8si));
            v8si vcmp = (vdata == vfirst);
            vresult &= vcmp;
        }

        int tmp[8];
        memcpy(tmp, &vresult, sizeof(tmp));
        for (int j = 0; j < 8; j++) {
            result &= (tmp[j] != 0);
        }
    }

    for (; i < n; i++) {
        result &= (first == a[i]);
    }

    int r = result ? 1 : 0;
    if (out) *out = r;
    return r;
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
