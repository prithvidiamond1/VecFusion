#include <stddef.h>

float vectorized_s314(int iterations, int LEN_1D, float* a) {
    float x = 0.0f;
    
    for (int nl = 0; nl < iterations * 5; nl++) {
        float max_val = a[0];
        
        // Vectorized reduction
        typedef float v4f __attribute__((vector_size(16)));
        const size_t vlen = 4;
        size_t i = 0;
        
        if (LEN_1D >= vlen) {
            v4f vmax = {a[0], a[0], a[0], a[0]};
            
            // Process vector chunks
            for (; i + vlen <= (size_t)LEN_1D; i += vlen) {
                v4f v = *(v4f*)&a[i];
                v4f cmp = v > vmax;
                vmax = (v & cmp) | (vmax & ~cmp);
            }
            
            // Horizontal reduction
            float vmax_arr[4];
            *(v4f*)vmax_arr = vmax;
            for (size_t j = 0; j < vlen; j++) {
                if (vmax_arr[j] > max_val) {
                    max_val = vmax_arr[j];
                }
            }
        }
        
        // Scalar tail
        for (; i < (size_t)LEN_1D; i++) {
            float v = a[i];
            if (v > max_val) {
                max_val = v;
            }
        }
        
        x = max_val;
    }
    
    return x;
}
