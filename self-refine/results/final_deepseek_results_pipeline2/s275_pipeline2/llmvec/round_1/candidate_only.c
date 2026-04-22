typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int nl_iterations = 10 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        // Store initial values of aa[0][i] before any modifications
        float init_aa0[256];
        for (int i = 0; i < 256; i++) {
            init_aa0[i] = aa[0][i];
        }
        
        // Process each column independently with vectorization
        for (int i = 0; i < 256; i++) {
            if (init_aa0[i] > 0.0f) {
                float prev = init_aa0[i];
                
                // Vectorized inner loop with explicit dependency chain
                // Process 4 elements at a time with scalar accumulation
                int j = 1;
                for (; j + 3 < 256; j += 4) {
                    // Load 4 bb and cc values
                    float4 bb_vec = {bb[j][i], bb[j+1][i], bb[j+2][i], bb[j+3][i]};
                    float4 cc_vec = {cc[j][i], cc[j+1][i], cc[j+2][i], cc[j+3][i]};
                    
                    // Compute 4 products
                    float4 products = bb_vec * cc_vec;
                    
                    // Scalar accumulation chain for dependency
                    prev = prev + products.x;
                    aa[j][i] = prev;
                    
                    prev = prev + products.y;
                    aa[j+1][i] = prev;
                    
                    prev = prev + products.z;
                    aa[j+2][i] = prev;
                    
                    prev = prev + products.w;
                    aa[j+3][i] = prev;
                }
                
                // Scalar tail cleanup
                for (; j < 256; j++) {
                    prev = prev + bb[j][i] * cc[j][i];
                    aa[j][i] = prev;
                }
            }
        }
    }
}
