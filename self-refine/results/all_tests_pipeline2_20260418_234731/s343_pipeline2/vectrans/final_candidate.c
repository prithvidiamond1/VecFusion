void s343_opt(int iterations, float *restrict flat_2d_array, 
              float aa[256][256], float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = -1;
        
        for (int i = 0; i < 256; i++) {
            // Vector-friendly mask computation
            int mask[256];
            int count = 0;
            
            #pragma omp simd reduction(+:count)
            for (int j = 0; j < 256; j++) {
                mask[j] = (bb[j][i] > 0.0f);
                count += mask[j];
            }
            
            // Prefix sum - can be optimized with SIMD prefix sum
            int write_pos = k + 1;
            for (int j = 0; j < 256; j++) {
                if (mask[j]) {
                    flat_2d_array[write_pos] = aa[j][i];
                    write_pos++;
                }
            }
            k = write_pos - 1;
        }
    }
}
