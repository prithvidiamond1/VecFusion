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
