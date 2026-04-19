#include <stdint.h>
#include <string.h>

typedef float v8f __attribute__((ext_vector_type(8)));
typedef int v8i __attribute__((ext_vector_type(8)));

void vectorized_s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int outer_loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Vectorize a[i] += b[i] * c[i] with 8-wide vectors
        int i = 0;
        for (; i + 7 < 256; i += 8) {
            v8f b_vec = *(v8f*)&b[i];
            v8f c_vec = *(v8f*)&c[i];
            v8f a_vec = *(v8f*)&a[i];
            a_vec += b_vec * c_vec;
            *(v8f*)&a[i] = a_vec;
        }
        // Scalar tail
        for (; i < 256; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Loop interchange preserved: j outer, i inner
        for (int j = 1; j < 256; j++) {
            i = 0;
            // Vectorize across i dimension
            for (; i + 7 < 256; i += 8) {
                v8f prev_aa = *(v8f*)&aa[j-1][i];
                v8f bb_vec = *(v8f*)&bb[j][i];
                v8f a_vec = *(v8f*)&a[i];
                v8f result = prev_aa + bb_vec * a_vec;
                *(v8f*)&aa[j][i] = result;
            }
            // Scalar tail
            for (; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
            }
        }
    }
}
