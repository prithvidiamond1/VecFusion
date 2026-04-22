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

    int val = pts[0];

    // Use GCC/Clang vector extensions for SIMD-style processing
    typedef int v8si __attribute__((vector_size(32)));  // 8 x int (256-bit / AVX)

    int i = 1;
    int simd_count = 8;

    // Vectorized loop: process 8 ints at a time
    if (count - 1 >= simd_count) {
        v8si vval = {val, val, val, val, val, val, val, val};

        for (; i + simd_count - 1 < count; i += simd_count) {
            v8si chunk;
            __builtin_memcpy(&chunk, &pts[i], sizeof(v8si));
            v8si cmp = (chunk != vval);
            // If any element differs, cmp will have a non-zero lane
            // Check each lane
            int *cmp_arr = (int *)&cmp;
            for (int j = 0; j < simd_count; j++) {
                if (cmp_arr[j] != 0) return false;
            }
        }
    }

    // Scalar cleanup tail
    for (; i < count; i++) {
        if (pts[i] != val) return false;
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
