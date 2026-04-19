#include <math.h>
#include <stdint.h>

float vectorized_s3113(int iterations, int LEN_1D, float* a) {
    float max;
    float current_max;

    for (int nl = 0; nl < iterations * 4; nl++) {
        current_max = fabsf(a[0]);

        // Vectorized reduction for the main loop
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        const int VF = 4;
        
        // Initialize vector maximum with the scalar current_max
        v4f vmax = (v4f){current_max, current_max, current_max, current_max};
        
        // Process vectorizable iterations
        for (; i + VF <= LEN_1D; i += VF) {
            v4f va = *(v4f*)&a[i];
            // Compute absolute values using bitwise AND to clear sign bit
            v4f vabs = __builtin_fabsf(va);
            
            // Compare and select maximum
            v4f cmp = vabs > vmax;
            vmax = cmp ? vabs : vmax;
        }

        // Reduce vector maximum to scalar
        float vmax_arr[VF];
        *(v4f*)vmax_arr = vmax;
        float vec_max = vmax_arr[0];
        for (int j = 1; j < VF; j++) {
            if (vmax_arr[j] > vec_max) {
                vec_max = vmax_arr[j];
            }
        }
        
        // Update current_max with vector reduction result
        if (vec_max > current_max) {
            current_max = vec_max;
        }

        // Process remaining elements
        for (; i < LEN_1D; i++) {
            float abs_val = fabsf(a[i]);
            if (abs_val > current_max) {
                current_max = abs_val;
            }
        }

        max = current_max;
    }

    return max;
}
