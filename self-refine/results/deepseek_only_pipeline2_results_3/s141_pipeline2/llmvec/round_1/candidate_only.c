#include <stddef.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_max = 200 * (iterations / 256);
    
    // Precompute k indices once
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = i * (i + 3) / 2;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }
    
    // Process outer loop
    for (int nl = 0; nl < nl_max; nl++) {
        // Loop distribution with temporary accumulation
        for (int i = 0; i < 256; i++) {
            float temp_accum[256 - i];
            
            // First: gather all bb values (fully vectorizable)
            // Canonical counted loop with explicit bounds
            int j_start = i;
            int j_end = 256;
            int j_count = j_end - j_start;
            
            // Vector-friendly loop with cleanup
            int j;
            for (j = j_start; j + 3 < j_end; j += 4) {
                temp_accum[j - i] = bb[j][i];
                temp_accum[j - i + 1] = bb[j + 1][i];
                temp_accum[j - i + 2] = bb[j + 2][i];
                temp_accum[j - i + 3] = bb[j + 3][i];
            }
            // Scalar cleanup
            for (; j < j_end; j++) {
                temp_accum[j - i] = bb[j][i];
            }
            
            // Second: accumulate using precomputed indices
            // Use local pointer for better aliasing analysis
            float *local_flat = flat_2d_array;
            const int *local_k_idx = &k_indices[i][i];
            
            // Canonical counted loop with explicit bounds
            int accum_count = 256 - i;
            
            // Vector-friendly loop with cleanup
            int idx;
            for (idx = 0; idx + 3 < accum_count; idx += 4) {
                int k0 = local_k_idx[idx];
                int k1 = local_k_idx[idx + 1];
                int k2 = local_k_idx[idx + 2];
                int k3 = local_k_idx[idx + 3];
                
                local_flat[k0] += temp_accum[idx];
                local_flat[k1] += temp_accum[idx + 1];
                local_flat[k2] += temp_accum[idx + 2];
                local_flat[k3] += temp_accum[idx + 3];
            }
            // Scalar cleanup
            for (; idx < accum_count; idx++) {
                local_flat[local_k_idx[idx]] += temp_accum[idx];
            }
        }
    }
}
