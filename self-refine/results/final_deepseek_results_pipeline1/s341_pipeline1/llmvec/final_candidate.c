#include <stddef.h>
#include <stdint.h>

typedef int int32x4_t __attribute__((ext_vector_type(4)));

void vectorized_s341(int iterations, int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        int i = 0;

        // Vectorized main loop
        for (; i + 4 <= LEN_1D; i += 4) {
            int32x4_t vb = *(int32x4_t*)(b + i);
            int32x4_t zero = (int32x4_t){0, 0, 0, 0};
            int32x4_t mask = vb > zero;

            // Process each element using mask bits
            for (int k = 0; k < 4; k++) {
                if (mask[k]) {
                    j++;
                    a[j] = b[i + k];
                }
            }
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) {
                j++;
                a[j] = b[i];
            }
        }
    }
}
