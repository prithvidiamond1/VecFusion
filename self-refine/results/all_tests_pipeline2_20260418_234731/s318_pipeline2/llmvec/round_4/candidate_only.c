#include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    float max = 0.0f;
    int index = 0;

    for (int nl = 0; nl < iterations/2; nl++) {
        int k = 0;
        float current_max = fabsf(a[0]);
        int current_index = 0;
        k += inc;

        float max_val = current_max;
        int max_idx = 0;
        int current_k = k;

        // Process in chunks of 4
        int i;
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            // Load 4 values with correct stride
            float v0 = fabsf(a[current_k]);
            current_k += inc;
            float v1 = fabsf(a[current_k]);
            current_k += inc;
            float v2 = fabsf(a[current_k]);
            current_k += inc;
            float v3 = fabsf(a[current_k]);
            current_k += inc;

            // Update maximum and index
            if (v0 > max_val) { max_val = v0; max_idx = i; }
            if (v1 > max_val) { max_val = v1; max_idx = i + 1; }
            if (v2 > max_val) { max_val = v2; max_idx = i + 2; }
            if (v3 > max_val) { max_val = v3; max_idx = i + 3; }
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            float abs_val = fabsf(a[current_k]);
            if (abs_val > max_val) {
                max_val = abs_val;
                max_idx = i;
            }
            current_k += inc;
        }

        // Update global max and index
        if (nl == 0 || max_val > max) {
            max = max_val;
            index = max_idx;
        }
    }

    return max + index + 1;
}
