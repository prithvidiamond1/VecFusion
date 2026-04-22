#include <stddef.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    typedef float v8f __attribute__((ext_vector_type(8)));
    
    int outer_loop_count = 10 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        float a0 = a[0];
        
        for (int i = 0; i < 256; i++) {
            // Scalar recurrence for a[j] values
            float a_prev = (i == 0) ? a0 : a[0];
            for (int j = 1; j < 256; j++) {
                a_prev = 1.0f - a_prev;
                a[j] = a_prev;
            }
            
            // Vectorized computation of aa[j][i]
            int j = 1;
            for (; j + 7 < 256; j += 8) {
                v8f a_vec = {a[j], a[j+1], a[j+2], a[j+3], 
                            a[j+4], a[j+5], a[j+6], a[j+7]};
                v8f d_vec = {d[j], d[j+1], d[j+2], d[j+3], 
                            d[j+4], d[j+5], d[j+6], d[j+7]};
                v8f bb_vec = {bb[j][i], bb[j+1][i], bb[j+2][i], bb[j+3][i],
                             bb[j+4][i], bb[j+5][i], bb[j+6][i], bb[j+7][i]};
                
                v8f result = a_vec + bb_vec * d_vec;
                
                aa[j][i] = result[0];
                aa[j+1][i] = result[1];
                aa[j+2][i] = result[2];
                aa[j+3][i] = result[3];
                aa[j+4][i] = result[4];
                aa[j+5][i] = result[5];
                aa[j+6][i] = result[6];
                aa[j+7][i] = result[7];
            }
            
            // Scalar tail cleanup
            for (; j < 256; j++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
            }
        }
    }
}
