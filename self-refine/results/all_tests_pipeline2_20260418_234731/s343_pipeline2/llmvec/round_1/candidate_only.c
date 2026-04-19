#include <stddef.h>

void vectorized_s343(int iterations, float *restrict flat_2d_array,
                     float aa[256][256], float bb[256][256])
{
    int total_outer = 10 * (iterations / 256);
    
    for (int nl = 0; nl < total_outer; nl++) {
        int k = -1;
        
        for (int i = 0; i < 256; i++) {
            int mask[256];
            int count = 0;
            
            // Vectorized mask computation
            for (int j = 0; j < 256; j += 8) {
                // Process 8 elements at a time
                int mask0 = (bb[j][i] > 0.0f);
                int mask1 = (bb[j+1][i] > 0.0f);
                int mask2 = (bb[j+2][i] > 0.0f);
                int mask3 = (bb[j+3][i] > 0.0f);
                int mask4 = (bb[j+4][i] > 0.0f);
                int mask5 = (bb[j+5][i] > 0.0f);
                int mask6 = (bb[j+6][i] > 0.0f);
                int mask7 = (bb[j+7][i] > 0.0f);
                
                mask[j] = mask0;
                mask[j+1] = mask1;
                mask[j+2] = mask2;
                mask[j+3] = mask3;
                mask[j+4] = mask4;
                mask[j+5] = mask5;
                mask[j+6] = mask6;
                mask[j+7] = mask7;
                
                count += mask0 + mask1 + mask2 + mask3 + 
                         mask4 + mask5 + mask6 + mask7;
            }
            
            // Scalar prefix sum and write
            int write_pos = k + 1;
            for (int j = 0; j < 256; j++) {
                if (mask[j]) {
                    flat_2d_array[write_pos] = aa[j][i];
                    write_pos++;
                }
            }
            k = write_pos - 1;
        }
    }
}
