#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s115(int iterations, float* a, float aa[256][256])
{
    int loop_count = 1000 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];  // Store a[j] in register to avoid repeated loads
            float* aa_row = aa[j];

            // Vectorizable inner loop - no loop-carried dependency
            for (int i = j + 1; i < 256; i++) {
                a[i] -= aa_row[i] * aj;
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    int loop_count = 1000 * (iterations / 256);

    typedef float v4f __attribute__((vector_size(16)));
    const int VEC_WIDTH = 4;

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            float* aa_row = aa[j];

            v4f vaj = (v4f){aj, aj, aj, aj};

            int i = j + 1;
            // Align to vector boundary
            for (; i < 256 && (i % VEC_WIDTH != 0); i++) {
                a[i] -= aa_row[i] * aj;
            }
            // Vectorized portion
            for (; i + VEC_WIDTH <= 256; i += VEC_WIDTH) {
                v4f va, vaa;
                __builtin_memcpy(&va,  a      + i, sizeof(v4f));
                __builtin_memcpy(&vaa, aa_row + i, sizeof(v4f));
                va = va - vaa * vaj;
                __builtin_memcpy(a + i, &va, sizeof(v4f));
            }
            // Scalar tail
            for (; i < 256; i++) {
                a[i] -= aa_row[i] * aj;
            }
        }
    }
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float aa_scalar[128]; float aa_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                s115(iterations, a_scalar, aa_scalar); vectorized_s115(iterations, a_vector, aa_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
