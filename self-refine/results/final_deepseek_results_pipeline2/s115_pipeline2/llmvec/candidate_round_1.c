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
            float aj = a[j];
            float* aa_j = aa[j];

            for (int i = j + 1; i < 256; i++) {
                a[i] -= aa_j[i] * aj;
            }
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    int loop_count = 1000 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            float* aa_j = aa[j];

            int i = j + 1;
            int end = 256;

            // Align start to vector boundary
            int start = i;
            while (start < end && ((uintptr_t)(a + start) & 15) != 0) {
                a[start] -= aa_j[start] * aj;
                start++;
            }

            // Vectorized main loop
            for (int k = start; k + 3 < end; k += 4) {
                float4 a_vec = *(float4*)(a + k);
                float4 aa_vec = *(float4*)(aa_j + k);
                float4 result = a_vec - aa_vec * aj;
                *(float4*)(a + k) = result;
            }

            // Scalar tail
            for (int k = start + ((end - start) & ~3); k < end; k++) {
                a[k] -= aa_j[k] * aj;
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
