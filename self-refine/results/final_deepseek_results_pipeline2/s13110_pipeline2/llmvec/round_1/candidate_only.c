#include <stdint.h>
#include <string.h>

float vectorized_s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_loop_count; nl++) {
        // First pass: find row-wise max values (vectorizable)
        float row_max_vals[256];
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            // Vector-friendly loop with explicit bounds
            int j = 1;
            // Process in chunks of 4 for better vectorization
            for (; j <= 252; j += 4) {
                float current0 = aa[i][j];
                float current1 = aa[i][j+1];
                float current2 = aa[i][j+2];
                float current3 = aa[i][j+3];
                
                if (current0 > row_max) row_max = current0;
                if (current1 > row_max) row_max = current1;
                if (current2 > row_max) row_max = current2;
                if (current3 > row_max) row_max = current3;
            }
            // Scalar cleanup for remaining elements
            for (; j < 256; j++) {
                float current = aa[i][j];
                if (current > row_max) {
                    row_max = current;
                }
            }
            row_max_vals[i] = row_max;
        }

        // Second pass: find global max value across rows (vectorizable)
        max = row_max_vals[0];
        // Process in chunks of 4
        int i = 1;
        for (; i <= 252; i += 4) {
            float val0 = row_max_vals[i];
            float val1 = row_max_vals[i+1];
            float val2 = row_max_vals[i+2];
            float val3 = row_max_vals[i+3];
            
            if (val0 > max) max = val0;
            if (val1 > max) max = val1;
            if (val2 > max) max = val2;
            if (val3 > max) max = val3;
        }
        // Scalar cleanup for remaining elements
        for (; i < 256; i++) {
            if (row_max_vals[i] > max) {
                max = row_max_vals[i];
            }
        }

        // Third pass: find first occurrence of global max (row-major order)
        // Convert to canonical counted loop with explicit bounds
        xindex = 0;
        yindex = 0;
        int found = 0;
        // Outer loop with explicit bounds
        for (int i = 0; i < 256; i++) {
            if (found) break;
            if (row_max_vals[i] == max) {
                // Inner loop with explicit bounds
                for (int j = 0; j < 256; j++) {
                    if (aa[i][j] == max) {
                        xindex = i;
                        yindex = j;
                        found = 1;
                        break;
                    }
                }
            }
        }

        chksum = max + (float)xindex + (float)yindex;
    }

    return max + (float)(xindex + 1) + (float)(yindex + 1);
}
