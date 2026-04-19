#include <stddef.h>

float vectorized_s312(int iterations, int LEN_1D, float* a) {
    float prod = (float)1.;
    
    for (int nl = 0; nl < 10 * iterations; nl++) {
        prod = (float)1.;
        
        // Vectorized accumulation
        typedef float v4f __attribute__((vector_size(16)));
        v4f vprod = (v4f){1.0f, 1.0f, 1.0f, 1.0f};
        
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        
        // Process 4 elements at a time
        for (; i < limit; i += 4) {
            v4f va = *(v4f*)&a[i];
            vprod = vprod * va;
        }
        
        // Horizontal reduction of vector accumulator
        float temp_prod = 1.0f;
        float* vprod_elements = (float*)&vprod;
        for (int j = 0; j < 4; j++) {
            temp_prod *= vprod_elements[j];
        }
        
        // Process remaining elements
        for (; i < LEN_1D; i++) {
            temp_prod *= a[i];
        }
        
        prod = temp_prod;
    }
    
    return prod;
}
