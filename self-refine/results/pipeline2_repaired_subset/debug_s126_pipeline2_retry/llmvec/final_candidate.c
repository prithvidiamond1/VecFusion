#include <stdint.h>

void vectorized_s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        int k = 1;
        
        for (int i = 0; i < 256; i++) {
            float prev_bb = bb[0][i];
            
            // Vectorized multiplication
            for (int j = 1; j < 256; j += 4) {
                int k0 = k - 1;
                int k1 = k;
                int k2 = k + 1;
                int k3 = k + 2;
                
                float mul0 = flat_2d_array[k0] * cc[j][i];
                float mul1 = flat_2d_array[k1] * cc[j+1][i];
                float mul2 = flat_2d_array[k2] * cc[j+2][i];
                float mul3 = flat_2d_array[k3] * cc[j+3][i];
                
                // Serial accumulation (preserve dependency)
                prev_bb = prev_bb + mul0;
                bb[j][i] = prev_bb;
                
                prev_bb = prev_bb + mul1;
                bb[j+1][i] = prev_bb;
                
                prev_bb = prev_bb + mul2;
                bb[j+2][i] = prev_bb;
                
                prev_bb = prev_bb + mul3;
                bb[j+3][i] = prev_bb;
                
                k += 4;
            }
            k++; // Extra increment after column
        }
    }
}
