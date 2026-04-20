#include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    int index;
    float max;

    for (int nl = 0; nl < iterations/2; nl++) {
        max = fabsf(a[0]);
        index = 0;

        float current_max = max;
        int current_index = 0;

        int i = 1;
        int loop_bound = LEN_1D;

        // Process elements in chunks of 4
        for (; i + 3 < loop_bound; i += 4) {
            // Compute indices with stride
            int k0 = i * inc;
            int k1 = (i + 1) * inc;
            int k2 = (i + 2) * inc;
            int k3 = (i + 3) * inc;

            // Load and compute absolute values
            float abs_val0 = fabsf(a[k0]);
            float abs_val1 = fabsf(a[k1]);
            float abs_val2 = fabsf(a[k2]);
            float abs_val3 = fabsf(a[k3]);

            // Compare all against current_max at chunk start
            float chunk_max = current_max;
            int chunk_index = current_index;
            
            if (abs_val0 > chunk_max) {
                chunk_max = abs_val0;
                chunk_index = i;
            }
            if (abs_val1 > chunk_max) {
                chunk_max = abs_val1;
                chunk_index = i + 1;
            }
            if (abs_val2 > chunk_max) {
                chunk_max = abs_val2;
                chunk_index = i + 2;
            }
            if (abs_val3 > chunk_max) {
                chunk_max = abs_val3;
                chunk_index = i + 3;
            }
            
            // Update global reduction
            if (chunk_max > current_max) {
                current_max = chunk_max;
                current_index = chunk_index;
            }
        }

        // Scalar tail cleanup
        for (; i < loop_bound; i++) {
            int k = i * inc;
            float abs_val = fabsf(a[k]);
            if (abs_val > current_max) {
                current_max = abs_val;
                current_index = i;
            }
        }

        index = current_index;
        max = current_max;

        // chksum computation (unused but preserved)
        float chksum = max + (float) index;
    }
    return max + index + 1;
}
