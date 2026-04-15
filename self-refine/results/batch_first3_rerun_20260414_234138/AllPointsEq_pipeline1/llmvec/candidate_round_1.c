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
    typedef int v8si __attribute__((vector_size(32)));

    int i = 1;

    // Vector width: 8 ints = 32 bytes (AVX-friendly, but also works with SSE via compiler)
    const int VEC_WIDTH = 8;

    if (count - 1 >= VEC_WIDTH) {
        v8si splat = {val, val, val, val, val, val, val, val};

        for (; i <= count - VEC_WIDTH; i += VEC_WIDTH) {
            v8si chunk;
            __builtin_memcpy(&chunk, pts + i, sizeof(v8si));
            v8si cmp = (chunk != splat);
            // If any lane is non-zero, they differ
            // Treat the vector as an array of ints to check
            int tmp[VEC_WIDTH];
            __builtin_memcpy(tmp, &cmp, sizeof(tmp));
            for (int j = 0; j < VEC_WIDTH; j++) {
                if (tmp[j] != 0) return false;
            }
        }
    }

    // Scalar tail
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
