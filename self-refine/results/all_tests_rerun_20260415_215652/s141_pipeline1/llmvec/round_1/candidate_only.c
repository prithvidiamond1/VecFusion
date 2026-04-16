#include <stdint.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 200*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            k = (i+1) * ((i+1) - 1) / 2 + (i+1)-1;
            // The inner loop has a loop-carried dependency on k (k += j+1),
            // so we precompute the k values for each j, then do the accumulation.
            // k at iteration j = k_start + sum_{m=i}^{j-1} (m+1)
            // = k_start + sum_{m=i+1}^{j} m
            // = k_start + (j*(j+1)/2 - i*(i+1)/2)
            // So k[j] = k_base + j*(j+1)/2 - i*(i+1)/2
            // where k_base = (i+1)*i/2 + i = i*(i+1)/2 + i = i*(i+3)/2
            // Let's verify: at j=i, k = (i+1)*i/2 + i = i*(i+1)/2 + i
            // k_base = i*(i+1)/2 + i
            // k[j] = i*(i+1)/2 + i + j*(j+1)/2 - i*(i+1)/2 = i + j*(j+1)/2
            // So k[j] = i + j*(j+1)/2
            
            // We can unroll the inner loop in chunks of 4
            int j = i;
            int limit = 256;
            
            // Scalar loop since k indices are non-contiguous (scatter operation)
            // Unroll by 4 for better instruction-level parallelism
            for (; j <= limit - 4; j += 4) {
                int k0 = i + j*(j+1)/2;
                int k1 = i + (j+1)*(j+2)/2;
                int k2 = i + (j+2)*(j+3)/2;
                int k3 = i + (j+3)*(j+4)/2;
                flat_2d_array[k0] += bb[j][i];
                flat_2d_array[k1] += bb[j+1][i];
                flat_2d_array[k2] += bb[j+2][i];
                flat_2d_array[k3] += bb[j+3][i];
            }
            // Scalar cleanup
            // Reset k to correct value for remaining iterations
            if (j < limit) {
                k = i + j*(j+1)/2;
                for (; j < limit; j++) {
                    flat_2d_array[k] += bb[j][i];
                    k += j+1;
                }
            }
        }
    }
}
