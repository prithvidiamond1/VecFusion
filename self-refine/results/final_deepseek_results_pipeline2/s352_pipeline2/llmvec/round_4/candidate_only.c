float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot = 0.0f;
    
    for (int nl = 0; nl < 8 * iterations; nl++) {
        float partial_dot = 0.0f;
        
        // Ensure main_len is non-negative
        int main_len = LEN_1D >= 0 ? LEN_1D - (LEN_1D % 8) : 0;
        if (main_len < 0) main_len = 0;
        
        // Vectorized processing with explicit unaligned loads
        int i = 0;
        for (; i < main_len; i += 8) {
            typedef float v8f __attribute__((vector_size(32), aligned(1)));
            
            // Use memcpy for safe unaligned loads
            v8f va, vb;
            __builtin_memcpy(&va, &a[i], sizeof(v8f));
            __builtin_memcpy(&vb, &b[i], sizeof(v8f));
            
            v8f vprod = va * vb;
            
            // Safe horizontal sum using union to avoid aliasing issues
            union {
                v8f vec;
                float arr[8];
            } u;
            u.vec = vprod;
            
            partial_dot += u.arr[0] + u.arr[1] + u.arr[2] + u.arr[3] +
                          u.arr[4] + u.arr[5] + u.arr[6] + u.arr[7];
        }
        
        // Scalar tail for remaining elements
        for (; i < LEN_1D; i++) {
            partial_dot += a[i] * b[i];
        }
        
        // Accumulate across outer iterations
        dot += partial_dot;
    }
    
    return dot;
}
