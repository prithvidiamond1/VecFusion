#include <stdint.h>
typedef float v4sf __attribute__((__vector_size__(16)));

float vectorized_s315(int iterations, int LEN_1D, float* a) {
    // First loop: initialize a[i] = (i * 7) % LEN_1D
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (float)((i * 7) % LEN_1D);
    }

    float x, chksum;
    int index;
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;
        
        // Vectorized max-finding loop
        int i = 0;
        if (LEN_1D >= 4) {
            v4sf vmax = {a[0], a[0], a[0], a[0]};
            int vindex[4] = {0, 0, 0, 0};
            for (; i <= LEN_1D - 4; i += 4) {
                v4sf va = *(v4sf*)(a + i);
                v4sf cmp = va > vmax;
                // Update max values
                vmax = __builtin_shufflevector(vmax, va, 0, 1, 2, 3);
                // Update indices
                for (int j = 0; j < 4; j++) {
                    if (va[j] > vmax[j]) {
                        vmax[j] = va[j];
                        vindex[j] = i + j;
                    }
                }
            }
            // Reduce vector to scalar
            for (int j = 0; j < 4; j++) {
                if (vmax[j] > x) {
                    x = vmax[j];
                    index = vindex[j];
                }
            }
        }
        // Scalar cleanup
        for (; i < LEN_1D; ++i) {
            if (a[i] > x) {
                x = a[i];
                index = i;
            }
        }
        chksum = x + (float)index;
    }
    return index + x + 1;
}
