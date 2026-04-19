#include <stddef.h>

void vectorized_s293(int iterations, int LEN_1D, float* a) {
    if (LEN_1D <= 0) return;
    
    for (int nl = 0; nl < 4 * iterations; nl++) {
        float temp = a[0];
        
        // Vectorized section
        typedef float v4f __attribute__((vector_size(16)));
        const size_t vlen = 4;
        size_t i = 0;
        
        // Broadcast temp to vector
        v4f vtemp = (v4f){temp, temp, temp, temp};
        
        // Process vector chunks
        for (; i + vlen <= (size_t)LEN_1D; i += vlen) {
            v4f* vptr = (v4f*)&a[i];
            *vptr = vtemp;
        }
        
        // Scalar tail
        for (; i < (size_t)LEN_1D; i++) {
            a[i] = temp;
        }
    }
}
