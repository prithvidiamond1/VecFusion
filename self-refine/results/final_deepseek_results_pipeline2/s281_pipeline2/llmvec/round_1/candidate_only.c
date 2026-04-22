typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    // Compute vectorized width
    const int VEC_WIDTH = 4;
    const int VEC_ITERS = LEN_1D / VEC_WIDTH;
    const int VEC_REMAIN = LEN_1D % VEC_WIDTH;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Allocate temporary array for x values
        float x[LEN_1D];
        
        // Vectorized computation of x[i] = a[LEN_1D-i-1] + b[i] * c[i]
        for (int i = 0; i < VEC_ITERS; i++) {
            int base_idx = i * VEC_WIDTH;
            
            // Load b[i] and c[i] vectors
            float4 b_vec = *(float4*)&b[base_idx];
            float4 c_vec = *(float4*)&c[base_idx];
            
            // Compute reverse indices for a[LEN_1D-i-1]
            int4 rev_idx = (int4){LEN_1D - base_idx - 1, 
                                  LEN_1D - base_idx - 2,
                                  LEN_1D - base_idx - 3,
                                  LEN_1D - base_idx - 4};
            
            // Gather a values from reverse indices
            float4 a_rev_vec = (float4){a[rev_idx.x], a[rev_idx.y], 
                                        a[rev_idx.z], a[rev_idx.w]};
            
            // Compute x = a_rev + b * c
            float4 x_vec = a_rev_vec + b_vec * c_vec;
            
            // Store x vector
            *(float4*)&x[base_idx] = x_vec;
        }
        
        // Scalar tail for x computation
        for (int i = VEC_ITERS * VEC_WIDTH; i < LEN_1D; i++) {
            x[i] = a[LEN_1D - i - 1] + b[i] * c[i];
        }
        
        // Vectorized update of a[i] = x[i] - 1.0 and b[i] = x[i]
        for (int i = 0; i < VEC_ITERS; i++) {
            int base_idx = i * VEC_WIDTH;
            
            // Load x vector
            float4 x_vec = *(float4*)&x[base_idx];
            
            // Compute a = x - 1.0
            float4 a_vec = x_vec - (float4)1.0f;
            
            // Store results
            *(float4*)&a[base_idx] = a_vec;
            *(float4*)&b[base_idx] = x_vec;
        }
        
        // Scalar tail for updates
        for (int i = VEC_ITERS * VEC_WIDTH; i < LEN_1D; i++) {
            a[i] = x[i] - 1.0f;
            b[i] = x[i];
        }
    }
}
