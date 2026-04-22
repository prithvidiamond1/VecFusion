#include <stddef.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float* aa_row = aa[0];
    
    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.f;
        float s_vals[256];
        
        // First loop: compute s values with sequential dependency
        // Process in chunks of 4 for better locality
        for (int i = 0; i < 256; i += 4) {
            // Process 4 elements sequentially to maintain dependency
            for (int j = 0; j < 4 && i + j < 256; j++) {
                int idx = i + j;
                float condition = a[idx] > 0.f ? 1.f : 0.f;
                float current_s = s;
                s = condition * (d[idx] * d[idx]) + (1.f - condition) * current_s;
                s_vals[idx] = s;
            }
        }
        
        // Second loop: compute b[i] - vectorizable
        for (int i = 0; i < 256; i += 4) {
            int remaining = 256 - i;
            int limit = remaining < 4 ? remaining : 4;
            
            for (int j = 0; j < limit; j++) {
                int idx = i + j;
                b[idx] = s_vals[idx] * c[idx] + d[idx];
            }
        }
        
        // Third loop: compute e[i] - vectorizable
        for (int i = 0; i < 256; i += 4) {
            int remaining = 256 - i;
            int limit = remaining < 4 ? remaining : 4;
            
            for (int j = 0; j < limit; j++) {
                int idx = i + j;
                e[idx] = (s_vals[idx] + 1.f) * aa_row[idx];
            }
        }
    }
}
