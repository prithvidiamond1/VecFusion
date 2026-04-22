#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s451(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int loop_count = iterations / 5;

    for (int nl = 0; nl < loop_count; nl++) {
        // Fused loop with temporary storage to enable vectorization
        // while maintaining single memory write per element
        for (int i = 0; i < LEN_1D; i++) {
            float temp = sinf(b[i]) + cosf(c[i]);
            a[i] = temp;
        }
    }
}

        #include <math.h>

typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s451(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int loop_count = iterations / 5;

    for (int nl = 0; nl < loop_count; nl++) {
        int i = 0;
        int limit = LEN_1D - 3;

        // Vectorized main loop
        for (; i < limit; i += 4) {
            float4 b_vec = *(float4*)&b[i];
            float4 c_vec = *(float4*)&c[i];

            float4 sin_b, cos_c;
            // Compute sin(b[i..i+3]) and cos(c[i..i+3]) element-wise
            sin_b.x = sinf(b_vec.x);
            sin_b.y = sinf(b_vec.y);
            sin_b.z = sinf(b_vec.z);
            sin_b.w = sinf(b_vec.w);

            cos_c.x = cosf(c_vec.x);
            cos_c.y = cosf(c_vec.y);
            cos_c.z = cosf(c_vec.z);
            cos_c.w = cosf(c_vec.w);

            float4 result = sin_b + cos_c;
            *(float4*)&a[i] = result;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = sinf(b[i]) + cosf(c[i]);
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s451(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s451(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
