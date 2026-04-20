#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;
        
        typedef float v4f __attribute__((vector_size(16)));
        v4f vdot = {0.0f, 0.0f, 0.0f, 0.0f};
        
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        
        for (; i < limit; i += 4) {
            v4f va = *(v4f*)&a[i];
            v4f vb = *(v4f*)&b[i];
            v4f vmul = va * vb;
            vdot += vmul;
        }
        
        dot += vdot[0] + vdot[1] + vdot[2] + vdot[3];
        
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}
