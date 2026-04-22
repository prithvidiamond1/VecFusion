#include <stddef.h>

void vectorized_s115(int iterations, float* a, float aa[256][256]) {
    int loop_count = 1000 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            int i = j + 1;
            
            // Vectorized inner loop
            for (; i + 3 < 256; i += 4) {
                a[i] -= aa[j][i] * aj;
                a[i + 1] -= aa[j][i + 1] * aj;
                a[i + 2] -= aa[j][i + 2] * aj;
                a[i + 3] -= aa[j][i + 3] * aj;
            }
            
            // Scalar cleanup tail
            for (; i < 256; i++) {
                a[i] -= aa[j][i] * aj;
            }
        }
    }
}
