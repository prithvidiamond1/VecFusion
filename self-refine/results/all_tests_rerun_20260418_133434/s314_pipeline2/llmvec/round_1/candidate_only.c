#include <stdint.h>
#include <float.h>

float vectorized_s314(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        float max_val = a[0];
        int i = 1;

        // Vector width: process 8 floats at a time
        #define VEC_WIDTH 8
        typedef float float8 __attribute__((vector_size(8 * sizeof(float))));

        // Initialize vector of max values
        float8 vmax = {max_val, max_val, max_val, max_val,
                       max_val, max_val, max_val, max_val};

        int limit = LEN_1D - VEC_WIDTH + 1;
        for (; i < limit; i += VEC_WIDTH) {
            float8 va;
            __builtin_memcpy(&va, &a[i], sizeof(float8));
            // Element-wise max using ternary via comparison
            float8 mask = va > vmax;
            // Use mask to select: mask is all-ones or all-zeros per element
            // Portable approach: use conditional assignment per lane
            vmax = (float8){
                (va[0] > vmax[0]) ? va[0] : vmax[0],
                (va[1] > vmax[1]) ? va[1] : vmax[1],
                (va[2] > vmax[2]) ? va[2] : vmax[2],
                (va[3] > vmax[3]) ? va[3] : vmax[3],
                (va[4] > vmax[4]) ? va[4] : vmax[4],
                (va[5] > vmax[5]) ? va[5] : vmax[5],
                (va[6] > vmax[6]) ? va[6] : vmax[6],
                (va[7] > vmax[7]) ? va[7] : vmax[7],
            };
        }

        // Reduce vector max to scalar
        float reduced = vmax[0];
        for (int k = 1; k < VEC_WIDTH; k++) {
            if (vmax[k] > reduced) reduced = vmax[k];
        }
        max_val = reduced;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }

        x = max_val;
    }
    return x;
}
