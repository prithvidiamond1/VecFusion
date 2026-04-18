#include <stdint.h>
#include <string.h>

void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    float s_sum = s1 + s2;
    int outer_iters = iterations / 5;

    for (int nl = 0; nl < outer_iters; nl++) {
        float prev_a = a[0];

        float* tmp = (float*)__builtin_alloca(LEN_1D * sizeof(float));

        // Vectorizable loop: compute independent sums using vector types
        typedef float float4 __attribute__((vector_size(16)));

        int i = 1;
        int vec_end = 1 + ((LEN_1D - 1) / 4) * 4;

        for (; i + 3 < LEN_1D; i += 4) {
            float4 vb, vc, vd, vr;
            memcpy(&vb, &b[i], 16);
            memcpy(&vc, &c[i], 16);
            memcpy(&vd, &d[i], 16);
            float4 vs = {s_sum, s_sum, s_sum, s_sum};
            vr = vs + vb + vc + vd;
            memcpy(&tmp[i], &vr, 16);
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            tmp[i] = s_sum + b[i] + c[i] + d[i];
        }

        // Recurrence: sequential prefix sum, cannot be vectorized directly
        for (int j = 1; j < LEN_1D; ++j) {
            prev_a = prev_a + tmp[j];
            a[j] = prev_a;
        }
    }
}
