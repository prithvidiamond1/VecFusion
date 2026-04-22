typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

float vectorized_s315(int iterations, int LEN_1D, float* a) {
    // Initialization loop - vectorized
    int i = 0;
    for (; i + 3 < LEN_1D; i += 4) {
        int4 indices = {i, i+1, i+2, i+3};
        int4 scaled = indices * 7;
        int4 mod_result = scaled % LEN_1D;
        float4 result = {(float)mod_result[0], (float)mod_result[1],
                         (float)mod_result[2], (float)mod_result[3]};
        *(float4*)&a[i] = result;
    }
    for (; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x = 0.0f;
    int index = 0;

    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // Vectorized max reduction
        float max_val = a[0];
        i = 0;
        
        // Handle initial elements if LEN_1D < 4
        if (LEN_1D >= 4) {
            float4 max_vec = *(float4*)&a[0];
            i = 4;
            
            for (; i + 3 < LEN_1D; i += 4) {
                float4 current_vec = *(float4*)&a[i];
                int4 cmp = current_vec > max_vec;
                // Manual blend using conditional operator per lane
                max_vec[0] = cmp[0] ? current_vec[0] : max_vec[0];
                max_vec[1] = cmp[1] ? current_vec[1] : max_vec[1];
                max_vec[2] = cmp[2] ? current_vec[2] : max_vec[2];
                max_vec[3] = cmp[3] ? current_vec[3] : max_vec[3];
            }
            
            // Extract scalar max from vector
            max_val = max_vec[0];
            if (max_vec[1] > max_val) max_val = max_vec[1];
            if (max_vec[2] > max_val) max_val = max_vec[2];
            if (max_vec[3] > max_val) max_val = max_vec[3];
        }

        // Scalar tail for max
        for (; i < LEN_1D; i++) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }

        // Find first index where value equals max_val
        index = 0;
        for (i = 0; i < LEN_1D; ++i) {
            if (a[i] == max_val) {
                index = i;
                break;
            }
        }

        x = max_val;
        float chksum = x + (float)index;
        (void)chksum;  // Suppress unused variable warning
    }

    return index + x + 1;
}
