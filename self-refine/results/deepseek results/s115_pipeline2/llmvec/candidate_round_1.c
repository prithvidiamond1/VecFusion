#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s115(int iterations, float* a, float aa[256][256])
{
    for (int nl = 0; nl < 1000*(iterations/256); nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            for (int i = j+1; i < 256; i++) {
                a[i] -= aa[j][i] * aj;
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    for (int nl = 0; nl < 1000*(iterations/256); nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            int i = j + 1;
            int limit = 256;

            // Vectorize using GCC/Clang vector extensions
            typedef float float4 __attribute__((vector_size(16)));
            typedef float float8 __attribute__((vector_size(32)));

            float8 vaj = { aj, aj, aj, aj, aj, aj, aj, aj };

            // Process 8 elements at a time
            for (; i + 7 < limit; i += 8) {
                float8 vai, vaa;
                __builtin_memcpy(&vai, &a[i], sizeof(float8));
                __builtin_memcpy(&vaa, &aa[j][i], sizeof(float8));
                vai = vai - vaa * vaj;
                __builtin_memcpy(&a[i], &vai, sizeof(float8));
            }

            // Process 4 elements at a time
            float4 vaj4 = { aj, aj, aj, aj };
            for (; i + 3 < limit; i += 4) {
                float4 vai4, vaa4;
                __builtin_memcpy(&vai4, &a[i], sizeof(float4));
                __builtin_memcpy(&vaa4, &aa[j][i], sizeof(float4));
                vai4 = vai4 - vaa4 * vaj4;
                __builtin_memcpy(&a[i], &vai4, sizeof(float4));
            }

            // Scalar tail
            for (; i < limit; i++) {
                a[i] -= aa[j][i] * aj;
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
