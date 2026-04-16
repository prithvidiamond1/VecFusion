#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        bool AllPointsEq(const int pts[], int count) {
    int ref = pts[0];
    int eq = 1;
    for (int i = 1; i < count; ++i) {
        eq &= (pts[i] == ref);
    }
    return (bool)eq;
}

        #include <stdbool.h>
#include <stdint.h>

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int ref = pts[0];

    // Use 4-wide int vector
    typedef int v4si __attribute__((vector_size(16)));

    v4si vref = {ref, ref, ref, ref};
    v4si vall = {1, 1, 1, 1};

    int i = 1;
    int limit = count - 3; // process 4 at a time starting from index 1

    // We need to be careful: we start at i=1, so we process pts[1..count-1]
    // For simplicity, align to groups of 4 from index 1
    for (; i + 3 < count; i += 4) {
        v4si chunk;
        chunk[0] = pts[i];
        chunk[1] = pts[i + 1];
        chunk[2] = pts[i + 2];
        chunk[3] = pts[i + 3];

        v4si cmp = (chunk == vref); // element-wise: -1 if equal, 0 if not
        // cmp elements are -1 (true) or 0 (false)
        // We want to AND all results: if any element is 0, result is false
        vall &= cmp;
    }

    // Check the accumulated vector result
    // All elements should be -1 (all bits set) if all were equal
    int result = (vall[0] & vall[1] & vall[2] & vall[3]);
    // result is -1 if all equal, 0 if any not equal
    // Convert: nonzero means all equal so far
    int eq = (result != 0) ? 1 : 0;

    // Scalar cleanup tail
    for (; i < count; ++i) {
        eq &= (pts[i] == ref);
    }

    return (bool)eq;
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
            const int n = 128;
            uint32_t seed = 7u;
            int pts[128]; int count = n; bool ret_scalar; bool ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_i32(pts, n, &seed);
                ret_scalar = AllPointsEq(pts, count); ret_vector = vectorized_AllPointsEq(pts, count); if (ret_scalar != ret_vector) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}

            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
