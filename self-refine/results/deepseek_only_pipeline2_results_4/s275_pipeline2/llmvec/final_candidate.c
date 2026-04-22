#include <stddef.h>

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256]) {
    typedef float v8f __attribute__((__vector_size__(32), __aligned__(32)));
    typedef int v8i __attribute__((__vector_size__(32), __aligned__(32)));
    
    const int outer_loop_count = 10 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Vectorized mask creation
        v8i mask_vec[32];
        for (int vi = 0; vi < 32; vi++) {
            int base = vi * 8;
            v8f a0_vec = *(v8f*)&aa[0][base];
            v8f zero_vec = (v8f){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
            v8i cmp_result = (v8i)(a0_vec > zero_vec);
            mask_vec[vi] = cmp_result;
        }
        
        // Process columns with vectorized mask-based computation
        for (int j = 1; j < 256; j++) {
            for (int vi = 0; vi < 32; vi++) {
                int base = vi * 8;
                v8i mask = mask_vec[vi];
                
                // Load data for vector lanes
                v8f aa_prev = *(v8f*)&aa[j-1][base];
                v8f bb_curr = *(v8f*)&bb[j][base];
                v8f cc_curr = *(v8f*)&cc[j][base];
                
                // Compute new values for all lanes
                v8f new_aa = aa_prev + bb_curr * cc_curr;
                
                // Blend with previous values based on mask
                v8f aa_curr = *(v8f*)&aa[j][base];
                v8f result = (v8f)((mask & (v8i)new_aa) | (~mask & (v8i)aa_curr));
                
                // Store result
                *(v8f*)&aa[j][base] = result;
            }
        }
    }
}
