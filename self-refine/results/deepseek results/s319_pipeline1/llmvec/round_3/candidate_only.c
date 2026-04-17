#include <stdint.h>

float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e)
{
    typedef float float4 __attribute__((vector_size(16)));

    float sum;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        sum = 0.0f;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);

        float4 vsum0 = {0.0f, 0.0f, 0.0f, 0.0f};

        for (; i < limit; i += 4) {
            float4 vc, vd, ve, va, vb;

            vc = (float4){c[i], c[i+1], c[i+2], c[i+3]};
            vd = (float4){d[i], d[i+1], d[i+2], d[i+3]};
            ve = (float4){e[i], e[i+1], e[i+2], e[i+3]};

            va = vc + vd;
            vb = vc + ve;

            a[i]   = va[0]; a[i+1] = va[1]; a[i+2] = va[2]; a[i+3] = va[3];
            b[i]   = vb[0]; b[i+1] = vb[1]; b[i+2] = vb[2]; b[i+3] = vb[3];

            vsum0 += va;
            vsum0 += vb;
        }

        sum = vsum0[0] + vsum0[1] + vsum0[2] + vsum0[3];

        for (; i < LEN_1D; i++) {
            a[i] = c[i] + d[i];
            sum += a[i];
            b[i] = c[i] + e[i];
            sum += b[i];
        }
    }
    return sum;
}
