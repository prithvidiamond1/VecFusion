typedef float float4 __attribute__((vector_size(16)));

void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    // Compute loop bounds for vectorized portion
    int start = 1;
    int end = LEN_1D - 1;
    int vec_len = end - start;
    
    // Process all iterations
    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized store of a[i+1] * d[i]
        int i = start;
        for (; i + 3 < end; i += 4) {
            float4 a_vec = {a[i+1], a[i+2], a[i+3], a[i+4]};
            float4 d_vec = {d[i], d[i+1], d[i+2], d[i+3]};
            float4 b_vec = a_vec * d_vec;
            b[i] = b_vec[0];
            b[i+1] = b_vec[1];
            b[i+2] = b_vec[2];
            b[i+3] = b_vec[3];
        }
        // Scalar tail for first loop
        for (; i < end; i++) {
            b[i] = a[i+1] * d[i];
        }
        
        // Vectorized computation of b[i-1] + c[i]
        i = start;
        for (; i + 3 < end; i += 4) {
            float4 b_prev = {b[i-1], b[i], b[i+1], b[i+2]};
            float4 c_vec = {c[i], c[i+1], c[i+2], c[i+3]};
            float4 a_vec = b_prev + c_vec;
            a[i] = a_vec[0];
            a[i+1] = a_vec[1];
            a[i+2] = a_vec[2];
            a[i+3] = a_vec[3];
        }
        // Scalar tail for second loop
        for (; i < end; i++) {
            a[i] = b[i-1] + c[i];
        }
    }
}
