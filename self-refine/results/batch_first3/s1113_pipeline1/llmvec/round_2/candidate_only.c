#include <stdint.h>

void vectorized_s1113(float* a, float* b, float* out, int LEN_1D)
{
    float scalar_val = a[LEN_1D/2];
    int i = 0;
    int limit = LEN_1D - (LEN_1D % 8);
    for (; i < limit; i += 8) {
        out[i+0] = scalar_val + b[i+0];
        out[i+1] = scalar_val + b[i+1];
        out[i+2] = scalar_val + b[i+2];
        out[i+3] = scalar_val + b[i+3];
        out[i+4] = scalar_val + b[i+4];
        out[i+5] = scalar_val + b[i+5];
        out[i+6] = scalar_val + b[i+6];
        out[i+7] = scalar_val + b[i+7];
    }
    for (; i < LEN_1D; i++) {
        out[i] = scalar_val + b[i];
    }
}
