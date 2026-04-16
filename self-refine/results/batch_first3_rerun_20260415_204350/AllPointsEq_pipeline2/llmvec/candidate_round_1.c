#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        bool AllPointsEq(const int pts[], int count) {
    int mismatch = 0;
    for (int i = 1; i < count; ++i) {
        mismatch |= (pts[0] != pts[i]);
    }
    return !mismatch;
}

        #include <stdbool.h>
#include <stdint.h>

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int val = pts[0];
    int mismatch = 0;

    typedef int v8si __attribute__((vector_size(32)));

    v8si vval = {val, val, val, val, val, val, val, val};
    v8si vmismatch = {0, 0, 0, 0, 0, 0, 0, 0};

    int i = 1;
    int vec_count = (count - 1) / 8;

    for (int v = 0; v < vec_count; ++v, i += 8) {
        v8si chunk;
        __builtin_memcpy(&chunk, &pts[i], sizeof(v8si));
        v8si cmp = (chunk != vval);
        vmismatch |= cmp;
    }

    /* Reduce vmismatch */
    int reduced = vmismatch[0] | vmismatch[1] | vmismatch[2] | vmismatch[3] |
                  vmismatch[4] | vmismatch[5] | vmismatch[6] | vmismatch[7];
    mismatch |= reduced;

    /* Scalar tail */
    for (; i < count; ++i) {
        mismatch |= (pts[0] != pts[i]);
    }

    return !mismatch;
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
