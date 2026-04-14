#include <stdint.h>
            #include <stdio.h>
            #include <stdlib.h>
            #include <string.h>
            #include <math.h>

            void s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        for (int i = LEN_1D - 2; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }
    }
}

            void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    // This loop has a loop-carried dependency: a[i+1] depends on a[i] in the previous iteration
    // (when iterating backwards, a[i+1] = a[i] + b[i], and next iteration uses a[i-1] + b[i-1])
    // Actually iterating i from LEN_1D-2 down to 0:
    //   a[LEN_1D-1] = a[LEN_1D-2] + b[LEN_1D-2]
    //   a[LEN_1D-2] = a[LEN_1D-3] + b[LEN_1D-3]
    //   ...
    //   a[1] = a[0] + b[0]
    // Each write to a[i+1] is not read again in the same sweep (since we go downward),
    // so there's no loop-carried dependency within one nl iteration.
    // We can vectorize the inner loop with care about the access pattern.
    // 
    // Since a[i+1] is written and a[i] is read, and i decreases, writes go to higher indices
    // while reads come from lower indices. No overlap within one sweep.
    // We can reverse the loop direction conceptually and vectorize.
    //
    // Rewrite: for i from LEN_1D-2 down to 0: a[i+1] = a[i] + b[i]
    // Equivalent: for j from 1 to LEN_1D-1: a[j] = a[j-1] + b[j-1]  (j = i+1)
    // But this forward version has a dependency: a[j] depends on a[j-1] which was just written.
    // The original backward version does NOT have this dependency since writes are to higher indices.
    // So we must keep the backward iteration order.
    //
    // With backward iteration, chunk reads from a[i..i+W-1] and writes to a[i+1..i+W],
    // but since i decreases, the write range [i+1..i+W] doesn't overlap with future read
    // ranges [i-W..i-1]. Safe to vectorize with chunks going downward.

    for (int nl = 0; nl < 3 * iterations; nl++) {
        int i = LEN_1D - 2;

        // Vector width of 8 floats
        #define VW 8
        // Process chunks of VW going downward
        // Chunk: i, i-1, ..., i-VW+1 => writes to i+1, i, ..., i-VW+2
        // reads a[i], a[i-1], ..., a[i-VW+1]
        // writes a[i+1], a[i], ..., a[i-VW+2]
        // Next chunk starts at i-VW, writes up to i-VW+2, no overlap with current writes (i-VW+2 < i-VW+2... boundary ok)
        // Actually need i-VW+2 > i-VW+1 which is always true, so no overlap between chunks.

        for (; i >= VW - 1; i -= VW) {
            // i, i-1, ..., i-VW+1 are the read indices for a
            // i+1, i, ..., i-VW+2 are the write indices
            float a0 = a[i]     + b[i];
            float a1 = a[i-1]   + b[i-1];
            float a2 = a[i-2]   + b[i-2];
            float a3 = a[i-3]   + b[i-3];
            float a4 = a[i-4]   + b[i-4];
            float a5 = a[i-5]   + b[i-5];
            float a6 = a[i-6]   + b[i-6];
            float a7 = a[i-7]   + b[i-7];
            a[i+1]   = a0;
            a[i]     = a1;
            a[i-1]   = a2;
            a[i-2]   = a3;
            a[i-3]   = a4;
            a[i-4]   = a5;
            a[i-5]   = a6;
            a[i-6]   = a7;
        }
        // Scalar cleanup
        for (; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }
        #undef VW
    }
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
