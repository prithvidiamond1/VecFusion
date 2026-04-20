#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        bool AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int first = pts[0];
    int all_eq = 1;

    // Process elements in blocks
    int i = 1;
    for (; i + 3 < count; i += 4) {
        int eq1 = (first == pts[i]);
        int eq2 = (first == pts[i + 1]);
        int eq3 = (first == pts[i + 2]);
        int eq4 = (first == pts[i + 3]);

        int block_eq = eq1 & eq2 & eq3 & eq4;
        all_eq &= block_eq;

        // Early exit if mismatch found
        if (!all_eq) return false;
    }

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
    int all_eq = 1;

    // Vectorized comparison
    const v4si first_vec = (v4si){first, first, first, first};
    int i = 1;

    // Process in chunks of 4 using vector operations
    for (; i + 3 < count; i += 4) {
        v4si chunk = *(const v4si*)(&pts[i]);
        v4si cmp_result = (chunk == first_vec);

        // Check if all 4 comparisons are true
        int mask = (cmp_result[0] & cmp_result[1] & 
                   cmp_result[2] & cmp_result[3]);
        all_eq &= mask;

        if (!all_eq) return false;
    }

    // Scalar tail handling
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
