#include <stdint.h>
#include <x86intrin.h>

typedef float v4sf __attribute__((__vector_size__(16)));

float vectorized_s312(int iterations, int LEN_1D, float* a) {
    float prod = 1.0f;
    int total_outer = 10 * iterations;
    for (int nl = 0; nl < total_outer; nl++) {
        v4sf vprod = {1.0f, 1.0f, 1.0f, 1.0f};
        int i = 0;
        if (LEN_1D >= 4) {
            for (; i <= LEN_1D - 4; i += 4) {
                v4sf va = (v4sf)__builtin_ia32_loadups(&a[i]);
                vprod = va * vprod;
            }
        }
        float prod_scalar = 1.0f;
        for (; i < LEN_1D; i++) {
            prod_scalar *= a[i];
        }
        prod = prod_scalar * vprod[0] * vprod[1] * vprod[2] * vprod[3];
    }
    return prod;
}
