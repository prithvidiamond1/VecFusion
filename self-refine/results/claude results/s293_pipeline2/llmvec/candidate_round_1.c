#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s293(int iterations, int LEN_1D, float* a)
{
    float val = a[0];
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = val;
    }
}

        #include <string.h>

void vectorized_s293(int iterations, int LEN_1D, float* a)
{
    float val = a[0];

    typedef float float4 __attribute__((vector_size(16)));

    float4 vval;
    vval[0] = val;
    vval[1] = val;
    vval[2] = val;
    vval[3] = val;

    int i = 0;
    int limit = LEN_1D - (LEN_1D % 4);

    for (; i < limit; i += 4) {
        __builtin_memcpy(a + i, &vval, 16);
    }

    for (; i < LEN_1D; i++) {
        a[i] = val;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                s293(iterations, LEN_1D, a_scalar); vectorized_s293(iterations, LEN_1D, a_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
