#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s453(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations*2; nl++) {
        // Remove the recurrence by computing s as 2.0*(i+1) directly
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = (2.0f * (i + 1)) * b[i];
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s453(int iterations, int LEN_1D, float* a, float *b) {
    for (int nl = 0; nl < iterations * 2; nl++) {
        int i = 0;
        const int vectorizable_elements = LEN_1D - (LEN_1D % 4);

        // Vectorized main loop
        for (; i < vectorizable_elements; i += 4) {
            float4 indices = {i + 1.0f, i + 2.0f, i + 3.0f, i + 4.0f};
            float4 scale = indices * 2.0f;

            float4 b_vec;
            b_vec[0] = b[i];
            b_vec[1] = b[i + 1];
            b_vec[2] = b[i + 2];
            b_vec[3] = b[i + 3];

            float4 result = scale * b_vec;

            a[i] = result[0];
            a[i + 1] = result[1];
            a[i + 2] = result[2];
            a[i + 3] = result[3];
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (2.0f * (i + 1)) * b[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s453(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s453(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
