#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < LEN_1D-1; ++i) {
            int neg = (c[i] < (float)0.);
            int pos = !neg;
            a[i] = a[i] * neg + (c[i] + d[i] * e[i]) * pos;
            b[i] = b[i] * pos + (a[i] + d[i] * d[i]) * neg;
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

        int vec_limit = limit - (limit % 4);
        for (; i < vec_limit; i += 4) {
            float4 ci, di, ei, ai, bi;
            // Load
            __builtin_memcpy(&ci, c + i, 16);
            __builtin_memcpy(&di, d + i, 16);
            __builtin_memcpy(&ei, e + i, 16);
            __builtin_memcpy(&ai, a + i, 16);
            __builtin_memcpy(&bi, b + i, 16);

            // neg mask: c[i] < 0.0f => -1 (all bits set), else 0
            // pos = !neg
            // Use vector comparison which yields 0 or -1 (all bits set)
            typedef int int4 __attribute__((vector_size(16)));

            int4 neg_mask = __builtin_convertvector(ci < (float4){0.f, 0.f, 0.f, 0.f}, int4);
            // neg_mask is -1 where true, 0 where false
            // neg as float: -1 -> 1.0f (we need 1.0 where neg is true)
            // We'll use bitwise AND with float representation

            // Convert masks to float: neg_mask & 1.0f bits
            float4 one = {1.f, 1.f, 1.f, 1.f};
            float4 zero = {0.f, 0.f, 0.f, 0.f};

            // neg_float = neg_mask ? 1.0f : 0.0f
            // pos_float = neg_mask ? 0.0f : 1.0f
            typedef int int4b __attribute__((vector_size(16)));
            int4b one_bits, zero_bits, neg_bits;
            __builtin_memcpy(&one_bits, &one, 16);
            __builtin_memcpy(&zero_bits, &zero, 16);

            neg_bits = neg_mask; // -1 or 0

            int4b neg_float_bits = neg_bits & one_bits;
            int4b pos_float_bits = (~neg_bits) & one_bits;

            float4 neg_float, pos_float;
            __builtin_memcpy(&neg_float, &neg_float_bits, 16);
            __builtin_memcpy(&pos_float, &pos_float_bits, 16);

            // a[i] = a[i] * neg + (c[i] + d[i] * e[i]) * pos
            float4 new_a = ai * neg_float + (ci + di * ei) * pos_float;

            // b[i] = b[i] * pos + (a[i] + d[i] * d[i]) * neg
            // Note: uses original a[i] before update? No, scalar uses updated a[i]
            // In scalar: a[i] is updated first, then b[i] uses new a[i]
            float4 new_b = bi * pos_float + (new_a + di * di) * neg_float;

            __builtin_memcpy(a + i, &new_a, 16);
            __builtin_memcpy(b + i, &new_b, 16);
        }
#endif

        // Scalar cleanup
        for (; i < limit; i++) {
            int neg = (c[i] < (float)0.);
            int pos = !neg;
            a[i] = a[i] * neg + (c[i] + d[i] * e[i]) * pos;
            b[i] = b[i] * pos + (a[i] + d[i] * d[i]) * neg;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s1161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s1161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
