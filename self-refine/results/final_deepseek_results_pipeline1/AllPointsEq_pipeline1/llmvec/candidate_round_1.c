#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        bool AllPointsEq(const int pts[], int count) {
    for (int i = 1; i < count; ++i) {
        if (pts[0] != pts[i]) {
            return false;
        }
    }
    return true;
}

        #include <stdbool.h>
#include <stdint.h>

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int first = pts[0];
    int i = 1;

    // Process 4 elements at a time using int vectors
    for (; i + 3 < count; i += 4) {
        int v0 = pts[i];
        int v1 = pts[i+1];
        int v2 = pts[i+2];
        int v3 = pts[i+3];
        if (first != v0 || first != v1 || first != v2 || first != v3) {
            return false;
        }
    }

    // Scalar cleanup for remaining elements
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
