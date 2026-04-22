#include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

void vectorized_s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int j = 1; j < 256; j++) {
            int i = 0;
            // Process 16 floats at a time (4 vectors of 4)
            for (; i <= 256 - 16; i += 16) {
                float4 bb_prev0 = *(float4*)(&bb[j-1][i+0]);
                float4 bb_prev1 = *(float4*)(&bb[j-1][i+4]);
                float4 bb_prev2 = *(float4*)(&bb[j-1][i+8]);
                float4 bb_prev3 = *(float4*)(&bb[j-1][i+12]);

                // flat_2d_array[i*256 + (j-1)] - note: indexed by i, not j
                // For each i, the index is i*256 + (j-1)
                // These are not contiguous in memory, so we gather manually
                float4 flat0 = {
                    flat_2d_array[(i+0)*256 + (j-1)],
                    flat_2d_array[(i+1)*256 + (j-1)],
                    flat_2d_array[(i+2)*256 + (j-1)],
                    flat_2d_array[(i+3)*256 + (j-1)]
                };
                float4 flat1 = {
                    flat_2d_array[(i+4)*256 + (j-1)],
                    flat_2d_array[(i+5)*256 + (j-1)],
                    flat_2d_array[(i+6)*256 + (j-1)],
                    flat_2d_array[(i+7)*256 + (j-1)]
                };
                float4 flat2 = {
                    flat_2d_array[(i+8)*256 + (j-1)],
                    flat_2d_array[(i+9)*256 + (j-1)],
                    flat_2d_array[(i+10)*256 + (j-1)],
                    flat_2d_array[(i+11)*256 + (j-1)]
                };
                float4 flat3 = {
                    flat_2d_array[(i+12)*256 + (j-1)],
                    flat_2d_array[(i+13)*256 + (j-1)],
                    flat_2d_array[(i+14)*256 + (j-1)],
                    flat_2d_array[(i+15)*256 + (j-1)]
                };

                float4 cc0 = *(float4*)(&cc[j][i+0]);
                float4 cc1 = *(float4*)(&cc[j][i+4]);
                float4 cc2 = *(float4*)(&cc[j][i+8]);
                float4 cc3 = *(float4*)(&cc[j][i+12]);

                *(float4*)(&bb[j][i+0])  = bb_prev0 + flat0 * cc0;
                *(float4*)(&bb[j][i+4])  = bb_prev1 + flat1 * cc1;
                *(float4*)(&bb[j][i+8])  = bb_prev2 + flat2 * cc2;
                *(float4*)(&bb[j][i+12]) = bb_prev3 + flat3 * cc3;
            }
            // Scalar cleanup
            for (; i < 256; i++) {
                bb[j][i] = bb[j-1][i] + flat_2d_array[i*256 + (j-1)] * cc[j][i];
            }
        }
    }
}
