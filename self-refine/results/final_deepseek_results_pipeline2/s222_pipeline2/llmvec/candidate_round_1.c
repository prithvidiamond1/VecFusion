#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s222(int iterations, int LEN_1D, float*a,float*b,float*c, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Split the loop: compute e[i] first (has loop-carried dependency)
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }
        // Then compute a[i] updates (no loop-carried dependency, vectorizable)
        for (int i = 1; i < LEN_1D; i++) {
            float temp_bc = b[i] * c[i];
            a[i] += temp_bc;
            a[i] -= temp_bc;
        }
    }
}

        typedef float float4 __attribute__((vector_size(16)));

void vectorized_s222(int iterations, int LEN_1D, float* a, float* b, float* c, float* e) {
    int outer_iters = iterations / 2;

    for (int nl = 0; nl < outer_iters; nl++) {
        // First loop: e[i] = e[i-1] * e[i-1] (sequential dependency)
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }

        // Second loop: vectorizable operations
        int i = 1;
        int limit = LEN_1D;

        // Vector loop
        for (; i + 3 < limit; i += 4) {
            float4 b_vec = __builtin_convertvector(*(float4*)&b[i], float4);
            float4 c_vec = __builtin_convertvector(*(float4*)&c[i], float4);
            float4 a_vec = __builtin_convertvector(*(float4*)&a[i], float4);

            float4 temp_bc = b_vec * c_vec;
            a_vec += temp_bc;
            a_vec -= temp_bc;

            *(float4*)&a[i] = __builtin_convertvector(a_vec, float4);
        }

        // Scalar tail
        for (; i < limit; i++) {
            float temp_bc = b[i] * c[i];
            a[i] += temp_bc;
            a[i] -= temp_bc;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s222(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, e_scalar); vectorized_s222(iterations, LEN_1D, a_vector, b_vector, c_vector, e_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
