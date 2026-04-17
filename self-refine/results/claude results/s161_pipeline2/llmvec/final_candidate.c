#include <string.h>
#include <stdint.h>

void vectorized_s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    float* c_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations/2; nl++) {
        memcpy(c_orig, c, LEN_1D * sizeof(float));

        int i = 0;
        int limit = LEN_1D - 1;

#if defined(__clang__) || defined(__GNUC__)
        typedef float float4 __attribute__((vector_size(16)));

        int limit4 = limit - (limit % 4);
        for (; i < limit4; i += 4) {
            float4 b_vec  = { b[i], b[i+1], b[i+2], b[i+3] };
            float4 zero   = { 0.f, 0.f, 0.f, 0.f };
            float4 one    = { 1.f, 1.f, 1.f, 1.f };

            // neg mask: b[i] < 0 => -1 (all bits set), else 0
            // We'll compute neg as float 1.0 or 0.0 manually
            float neg0 = (b[i]   < 0.f) ? 1.f : 0.f;
            float neg1 = (b[i+1] < 0.f) ? 1.f : 0.f;
            float neg2 = (b[i+2] < 0.f) ? 1.f : 0.f;
            float neg3 = (b[i+3] < 0.f) ? 1.f : 0.f;

            float pos0 = 1.f - neg0;
            float pos1 = 1.f - neg1;
            float pos2 = 1.f - neg2;
            float pos3 = 1.f - neg3;

            float4 neg_vec = { neg0, neg1, neg2, neg3 };
            float4 pos_vec = { pos0, pos1, pos2, pos3 };

            float4 c_orig_vec = { c_orig[i], c_orig[i+1], c_orig[i+2], c_orig[i+3] };
            float4 d_vec      = { d[i], d[i+1], d[i+2], d[i+3] };
            float4 e_vec      = { e[i], e[i+1], e[i+2], e[i+3] };
            float4 a_vec      = { a[i], a[i+1], a[i+2], a[i+3] };
            float4 c1_vec     = { c[i+1], c[i+2], c[i+3], c[i+4] };

            // a[i] = pos * (c_orig[i] + d[i]*e[i]) + neg * a[i]
            float4 new_a = pos_vec * (c_orig_vec + d_vec * e_vec) + neg_vec * a_vec;

            // c[i+1] = neg * (a[i] + d[i]*d[i]) + pos * c[i+1]
            // Note: use original a[i] (a_vec) here, not new_a
            float4 new_c1 = neg_vec * (a_vec + d_vec * d_vec) + pos_vec * c1_vec;

            a[i]   = new_a[0]; a[i+1] = new_a[1]; a[i+2] = new_a[2]; a[i+3] = new_a[3];
            c[i+1] = new_c1[0]; c[i+2] = new_c1[1]; c[i+3] = new_c1[2]; c[i+4] = new_c1[3];
        }
#endif

        // Scalar tail
        for (; i < limit; i++) {
            int neg = (b[i] < 0.f);
            int pos = 1 - neg;
            float a_old = a[i];
            a[i]   = pos * (c_orig[i] + d[i] * e[i]) + neg * a_old;
            c[i+1] = neg * (a_old + d[i] * d[i])     + pos * c[i+1];
        }
    }
}
