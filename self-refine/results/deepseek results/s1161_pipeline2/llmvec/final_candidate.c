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
