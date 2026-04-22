#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Process all i: compute both potential results
        for (int i = 0; i < LEN_1D-1; ++i) {
            float d_sq = d[i] * d[i];
            float de = d[i] * e[i];

            // Compute both branch results
            float a_new = c[i] + de;
            float b_new = a[i] + d_sq;

            // Mask-based selection without branching
            int cond = c[i] < 0.0f;
            a[i] = cond ? a[i] : a_new;      // Only update a if c[i] >= 0
            b[i] = cond ? b_new : b[i];      // Only update b if c[i] < 0
        }
    }
}

        #include <stdint.h>

void vectorized_s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        int limit = LEN_1D - 1;

#if defined(__clang__) || defined(__GNUC__)
        typedef float float4 __attribute__((vector_size(16)));
        typedef int   int4   __attribute__((vector_size(16)));

        int limit4 = limit - (limit % 4);
        for (; i < limit4; i += 4) {
            float4 ci, di, ei, ai, bi;

            // Load 4 elements
            __builtin_memcpy(&ci, c + i, 16);
            __builtin_memcpy(&di, d + i, 16);
            __builtin_memcpy(&ei, e + i, 16);
            __builtin_memcpy(&ai, a + i, 16);
            __builtin_memcpy(&bi, b + i, 16);

            float4 zero = {0.0f, 0.0f, 0.0f, 0.0f};

            float4 d_sq = di * di;
            float4 de   = di * ei;

            float4 a_new = ci + de;
            float4 b_new = ai + d_sq;

            // cond = c[i] < 0.0f  (all-ones if true, all-zeros if false)
            int4 cond = (int4)(ci < zero);

            // a[i] = cond ? a[i] : a_new  => select a_new when cond==0
            // b[i] = cond ? b_new : b[i]  => select b_new when cond!=0
            // Using bitwise blend: result = (cond & true_val) | (~cond & false_val)
            int4 ai_int    = (int4)ai;
            int4 bi_int    = (int4)bi;
            int4 a_new_int = (int4)a_new;
            int4 b_new_int = (int4)b_new;

            // a[i] = cond ? a[i] : a_new
            int4 a_result = (cond & ai_int) | (~cond & a_new_int);
            // b[i] = cond ? b_new : b[i]
            int4 b_result = (cond & b_new_int) | (~cond & bi_int);

            float4 a_out = (float4)a_result;
            float4 b_out = (float4)b_result;

            __builtin_memcpy(a + i, &a_out, 16);
            __builtin_memcpy(b + i, &b_out, 16);
        }
#endif

        // Scalar cleanup tail
        for (; i < limit; i++) {
            float d_sq = d[i] * d[i];
            float de   = d[i] * e[i];

            float a_new = c[i] + de;
            float b_new = a[i] + d_sq;

            int cond = c[i] < 0.0f;
            a[i] = cond ? a[i] : a_new;
            b[i] = cond ? b_new : b[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s1161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s1161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
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
