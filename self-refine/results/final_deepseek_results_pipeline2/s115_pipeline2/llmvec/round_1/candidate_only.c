typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    int loop_count = 1000 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            float* aa_j = aa[j];
            
            int i = j + 1;
            int end = 256;
            
            // Align start to vector boundary
            int start = i;
            while (start < end && ((uintptr_t)(a + start) & 15) != 0) {
                a[start] -= aa_j[start] * aj;
                start++;
            }
            
            // Vectorized main loop
            for (int k = start; k + 3 < end; k += 4) {
                float4 a_vec = *(float4*)(a + k);
                float4 aa_vec = *(float4*)(aa_j + k);
                float4 result = a_vec - aa_vec * aj;
                *(float4*)(a + k) = result;
            }
            
            // Scalar tail
            for (int k = start + ((end - start) & ~3); k < end; k++) {
                a[k] -= aa_j[k] * aj;
            }
        }
    }
}
