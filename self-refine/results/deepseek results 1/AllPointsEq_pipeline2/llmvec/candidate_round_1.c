#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        bool AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    const int first = pts[0];
    int i = 1;

    // Vectorization-friendly main loop: accumulate mismatches
    int any_mismatch = 0;
    for (; i + 3 < count; i += 4) {
        any_mismatch |= (first != pts[i]);
        any_mismatch |= (first != pts[i+1]);
        any_mismatch |= (first != pts[i+2]);
        any_mismatch |= (first != pts[i+3]);
    }

    // Early exit after vectorized portion
    if (any_mismatch) return false;

    // Cleanup loop for remaining elements
    for (; i < count; ++i) {
        if (first != pts[i]) {
            return false;
        }
    }

    return true;
}

        #include <stdbool.h>
#include <stdint.h>

typedef int v4si __attribute__((vector_size(16)));

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    const int first = pts[0];
    int i = 1;

    // Build a vector of 'first' repeated 4 times
    v4si vfirst = {first, first, first, first};
    v4si vmismatch = {0, 0, 0, 0};

    // Vectorized main loop: process 4 elements at a time
    for (; i + 3 < count; i += 4) {
        v4si vpts;
        __builtin_memcpy(&vpts, &pts[i], sizeof(v4si));
        // Compare: result is -1 (all bits set) where equal, 0 where not equal
        v4si vcmp = (vfirst != vpts);
        vmismatch |= vcmp;
    }

    // Check if any mismatch was found in vectorized portion
    // vmismatch elements are non-zero if there was a mismatch
    if (vmismatch[0] | vmismatch[1] | vmismatch[2] | vmismatch[3]) {
        return false;
    }

    // Scalar cleanup tail for remaining elements
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
