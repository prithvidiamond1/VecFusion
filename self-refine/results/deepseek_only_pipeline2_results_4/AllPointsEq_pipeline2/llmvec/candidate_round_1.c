#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        bool AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int first = pts[0];
    int i = 1;
    int found_mismatch = 0;

    // Main vectorizable loop without early exit
    for (; i + 3 < count; i += 4) {
        // Independent comparisons - compute mismatch mask
        int cmp0 = (first != pts[i]);
        int cmp1 = (first != pts[i + 1]);
        int cmp2 = (first != pts[i + 2]);
        int cmp3 = (first != pts[i + 3]);

        // Accumulate results without branching
        found_mismatch |= (cmp0 | cmp1 | cmp2 | cmp3);
    }

    // Early exit after vectorized portion
    if (found_mismatch) return false;

    // Handle remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }

    return true;
}

        #include <stdbool.h>
#include <stddef.h>

typedef int v4si __attribute__((vector_size(16)));

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int first = pts[0];
    int i = 1;
    int found_mismatch = 0;

    // Vectorized main loop
    for (; i + 3 < count; i += 4) {
        // Load 4 consecutive elements
        v4si chunk = *(const v4si*)(&pts[i]);
        // Broadcast first to vector
        v4si first_vec = (v4si){first, first, first, first};
        // Compare for inequality
        v4si cmp_vec = first_vec != chunk;
        // Check if any element is non-zero
        int any_mismatch = (cmp_vec[0] | cmp_vec[1] | cmp_vec[2] | cmp_vec[3]);
        found_mismatch |= any_mismatch;
    }

    if (found_mismatch) return false;

    // Scalar tail
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }

    return true;
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
            int pts[128]; int count = arr_len; bool ret_scalar; bool ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_i32(pts, arr_len, &seed);
                ret_scalar = AllPointsEq(pts, count); ret_vector = vectorized_AllPointsEq(pts, count); if (ret_scalar != ret_vector) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}

            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
