#include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    int index = 0;
    float max = 0.0f;

    for (int nl = 0; nl < iterations/2; nl++) {
        // Determine valid iteration range based on stride
        int start = 0;
        int end = LEN_1D;
        int step = 1;
        
        if (inc > 0) {
            // Positive stride: i*inc must be < LEN_1D
            end = (LEN_1D + inc - 1) / inc;  // ceil division
            if (end > LEN_1D) end = LEN_1D;
        } else if (inc < 0) {
            // Negative stride: i*inc must be >= 0
            end = LEN_1D / (-inc);  // integer division
            if (end > LEN_1D) end = LEN_1D;
        } else {
            // inc == 0: only access a[0]
            end = 1;
        }

        // Initialize with first valid element
        int first_idx = 0;
        max = fabsf(a[first_idx]);
        index = first_idx;

        float current_max = max;
        int current_index = first_idx;

        int i = 1;  // Start from second element in the sequence
        // Process elements in chunks of 4
        for (; i + 3 < end; i += 4) {
            // Compute actual array indices with stride
            int k0 = i * inc;
            int k1 = (i + 1) * inc;
            int k2 = (i + 2) * inc;
            int k3 = (i + 3) * inc;

            // Load and compute absolute values
            float abs_val0 = fabsf(a[k0]);
            float abs_val1 = fabsf(a[k1]);
            float abs_val2 = fabsf(a[k2]);
            float abs_val3 = fabsf(a[k3]);

            // Sequential reduction preserving first occurrence semantics
            if (abs_val0 > current_max) {
                current_max = abs_val0;
                current_index = k0;
            }
            if (abs_val1 > current_max) {
                current_max = abs_val1;
                current_index = k1;
            }
            if (abs_val2 > current_max) {
                current_max = abs_val2;
                current_index = k2;
            }
            if (abs_val3 > current_max) {
                current_max = abs_val3;
                current_index = k3;
            }
        }

        // Scalar tail cleanup with same bound
        for (; i < end; i++) {
            int k = i * inc;
            float abs_val = fabsf(a[k]);
            if (abs_val > current_max) {
                current_max = abs_val;
                current_index = k;
            }
        }

        // Update final results
        max = current_max;
        index = current_index;

        // chksum computation (unused but preserved)
        float chksum = max + (float) index;
    }

    return max + index + 1;
}
