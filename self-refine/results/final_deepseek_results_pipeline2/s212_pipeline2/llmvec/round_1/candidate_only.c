typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    if (n < 2) return;
    
    int limit = n - 1;
    int i = 0;
    
    // Vectorized main loop
    for (; i + 4 <= limit; i += 4) {
        // Load current and next a values
        float4 a_curr = {a[i], a[i+1], a[i+2], a[i+3]};
        float4 a_next = {a[i+1], a[i+2], a[i+3], a[i+4]};
        
        // Load c and d values
        float4 c_vec = {c[i], c[i+1], c[i+2], c[i+3]};
        float4 d_vec = {d[i], d[i+1], d[i+2], d[i+3]};
        
        // Compute: a[i] *= c[i]
        a_curr = a_curr * c_vec;
        
        // Store updated a values
        a[i] = a_curr[0];
        a[i+1] = a_curr[1];
        a[i+2] = a_curr[2];
        a[i+3] = a_curr[3];
        
        // Compute: b[i] += a_next_original * d[i]
        float4 b_vec = {b[i], b[i+1], b[i+2], b[i+3]};
        b_vec = b_vec + (a_next * d_vec);
        
        // Store updated b values
        b[i] = b_vec[0];
        b[i+1] = b_vec[1];
        b[i+2] = b_vec[2];
        b[i+3] = b_vec[3];
    }
    
    // Scalar cleanup for remaining iterations
    for (; i < limit; i++) {
        float a_next_original = a[i + 1];
        a[i] *= c[i];
        b[i] += a_next_original * d[i];
    }
}
