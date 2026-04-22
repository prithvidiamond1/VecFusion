typedef float __attribute__((ext_vector_type(4))) float4;
typedef int __attribute__((ext_vector_type(4))) int4;

float vectorized_s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max;
    int nl_limit = 100 * (iterations / 256);
    if (nl_limit < 1) nl_limit = 1;
    
    for (int nl = 0; nl < nl_limit; nl++) {
        float4 max_vec = {aa[0][0], aa[0][0], aa[0][0], aa[0][0]};
        int xindex_vec[4] = {0, 0, 0, 0};
        int yindex_vec[4] = {0, 0, 0, 0};

        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j += 4) {
                float4 *ptr = (float4 *)__builtin_assume_aligned(&aa[i][j], 16);
                float4 current = *ptr;
                int4 mask = current > max_vec;
                
                // Element-wise max using scalar fallback
                for (int k = 0; k < 4; k++) {
                    if (mask[k]) {
                        max_vec[k] = current[k];
                        xindex_vec[k] = i;
                        yindex_vec[k] = j + k;
                    }
                }
            }
        }

        // Reduce across vector lanes
        float max_val = max_vec[0];
        int xindex_val = xindex_vec[0];
        int yindex_val = yindex_vec[0];
        for (int k = 1; k < 4; k++) {
            if (max_vec[k] > max_val) {
                max_val = max_vec[k];
                xindex_val = xindex_vec[k];
                yindex_val = yindex_vec[k];
            }
        }

        max = max_val;
        xindex = xindex_val;
        yindex = yindex_val;
    }
    return max + xindex + 1 + yindex + 1;
}
