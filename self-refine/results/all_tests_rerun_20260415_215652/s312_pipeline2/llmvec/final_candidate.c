#include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

float vectorized_s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    for (int nl = 0; nl < 10 * iterations; nl++) {
        float4 vprod = {1.0f, 1.0f, 1.0f, 1.0f};

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);

        for (i = 0; i < limit; i += 4) {
            float4 va = {a[i], a[i+1], a[i+2], a[i+3]};
            vprod *= va;
        }

        float prod0 = vprod[0];
        float prod1 = vprod[1];
        float prod2 = vprod[2];
        float prod3 = vprod[3];

        for (; i < LEN_1D; i++) {
            prod0 *= a[i];
        }

        prod = prod0 * prod1 * prod2 * prod3;
    }
    return prod;
}
