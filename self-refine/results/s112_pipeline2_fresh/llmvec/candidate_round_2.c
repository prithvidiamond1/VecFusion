#include <stdint.h>
            #include <stdio.h>
            #include <stdlib.h>
            #include <string.h>
            #include <math.h>

            void s112_opt(float *a, float *b, int iterations, int LEN_1D) {
    float *tmp = (float *)malloc(LEN_1D * sizeof(float));
    for (int nl = 0; nl < 3 * iterations; nl++) {
        memcpy(tmp, a, LEN_1D * sizeof(float));
        for (int i = 0; i <= LEN_1D - 2; i++) {
            a[i + 1] = tmp[i] + b[i];
        }
    }
    free(tmp);
}

            #include <stdlib.h>
#include <string.h>

void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    float *tmp = (float *)malloc(LEN_1D * sizeof(float));
    for (int nl = 0; nl < 3 * iterations; nl++) {
        memcpy(tmp, a, LEN_1D * sizeof(float));

        int i = 0;
        int limit = LEN_1D - 2;

        /* Vectorized portion: process 8 elements at a time */
        for (; i <= limit - 7; i += 8) {
            a[i + 1] = tmp[i]     + b[i];
            a[i + 2] = tmp[i + 1] + b[i + 1];
            a[i + 3] = tmp[i + 2] + b[i + 2];
            a[i + 4] = tmp[i + 3] + b[i + 3];
            a[i + 5] = tmp[i + 4] + b[i + 4];
            a[i + 6] = tmp[i + 5] + b[i + 5];
            a[i + 7] = tmp[i + 6] + b[i + 6];
            a[i + 8] = tmp[i + 7] + b[i + 7];
        }

        /* Scalar cleanup tail */
        for (; i <= limit; i++) {
            a[i + 1] = tmp[i] + b[i];
        }
    }
    free(tmp);
}

void s112(float *a, float *b, int iterations, int LEN_1D) {
    vectorized_s112(a, b, iterations, LEN_1D);
}

            static uint32_t next_u32(uint32_t *state) {
                *state = (*state * 1664525u) + 1013904223u;
                return *state;
            }

            static void fill_f32(float *buf, int n, uint32_t *state) {
                for (int i = 0; i < n; ++i) {
                    buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
                }
            }

            static uint64_t checksum_f32(const float *buf, int n) {
                uint64_t acc = 1469598103934665603ull;
                for (int i = 0; i < n; ++i) {
                    union { float f; uint32_t u; } x;
                    x.f = buf[i];
                    acc ^= x.u;
                    acc *= 1099511628211ull;
                }
                return acc;
            }

            int main(void) {
                const int n = 128;
                const int iterations = 5;
                uint32_t seed = 7u;

                float a_scalar[128];
                float a_vector[128];
                float b[128];

                for (int trial = 0; trial < 64; ++trial) {
                    fill_f32(a_scalar, n, &seed);
                    memcpy(a_vector, a_scalar, sizeof(a_scalar));
                    fill_f32(b, n, &seed);

                    s112(a_scalar, b, iterations, n);
                    vectorized_s112(a_vector, b, iterations, n);

                    for (int i = 0; i < n; ++i) {
                        if (fabsf(a_scalar[i] - a_vector[i]) > 1e-5f) {
                            fprintf(stderr, "Mismatch on trial %d\n", trial);
                            fprintf(stderr, "scalar_checksum=%llu\n",
                                    (unsigned long long)checksum_f32(a_scalar, n));
                            fprintf(stderr, "vector_checksum=%llu\n",
                                    (unsigned long long)checksum_f32(a_vector, n));
                            fprintf(stderr, "first_diff_index=%d scalar=%f vector=%f\n",
                                    i, a_scalar[i], a_vector[i]);
                            return 2;
                        }
                    }
                }

                printf("PASS trials=%d checksum=%llu\n",
                       64,
                       (unsigned long long)checksum_f32(a_vector, n));
                return 0;
            }
