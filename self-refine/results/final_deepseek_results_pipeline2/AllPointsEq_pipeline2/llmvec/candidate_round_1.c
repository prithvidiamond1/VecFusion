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

    // Loop distribution: separate comparison from early exit
    // First, process in blocks without branching
    int remaining = count - 1;
    int block_size = 4;
    int vectorizable_iters = remaining / block_size;
    int vectorized_end = vectorizable_iters * block_size + 1;

    // Remove branching logic using arithmetic masking
    int all_equal = 1;  // Start assuming all equal

    // Main vectorizable loop - no early exit inside
    // Reorder: cluster memory accesses first, then compute comparisons
    for (; i < vectorized_end; i += block_size) {
        // Cluster similar memory accesses
        int val0, val1, val2, val3;
        val0 = pts[i];
        val1 = pts[i+1];
        val2 = pts[i+2];
        val3 = pts[i+3];

        // Compute equality mask without branching
        int eq0 = (first == val0);
        int eq1 = (first == val1);
        int eq2 = (first == val2);
        int eq3 = (first == val3);

        // Accumulate result using pure arithmetic reduction
        // This helps compiler recognize reduction pattern
        int block_equal = eq0 & eq1 & eq2 & eq3;
        all_equal = all_equal & block_equal;
    }

    // Early exit check after vector loop
    if (!all_equal) return false;

    // Cleanup loop (non-vectorizable tail)
    for (; i < count; ++i) {
        if (first != pts[i]) return false;
    }

    return true;
}

        #include <stdbool.h>
#include <stdint.h>

typedef int32_t v4si __attribute__((vector_size(16), aligned(1)));

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int first = pts[0];
    int i = 1;

    // Vectorized main loop
    int remaining = count - 1;
    int vectorizable_iters = remaining / 4;
    int vectorized_end = vectorizable_iters * 4 + 1;

    // Load first value into all lanes of vector
    v4si first_vec = (v4si){first, first, first, first};

    for (; i < vectorized_end; i += 4) {
        // Load 4 consecutive integers
        v4si chunk;
        // Use memcpy to avoid strict aliasing issues
        __builtin_memcpy(&chunk, &pts[i], sizeof(v4si));

        // Compare all 4 values with first
        v4si cmp_result = (chunk == first_vec);

        // Check if all comparisons are true (all bits set)
        // Extract comparison results
        int cmp0 = cmp_result[0];
        int cmp1 = cmp_result[1];
        int cmp2 = cmp_result[2];
        int cmp3 = cmp_result[3];

        // Early exit if any comparison fails
        if (!(cmp0 && cmp1 && cmp2 && cmp3)) {
            return false;
        }
    }

    // Scalar cleanup for remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) return false;
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
