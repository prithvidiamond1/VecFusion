typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(32)));

float vectorized_s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    const int CHUNK_SIZE = 64;
    
    for (int nl = 0; nl < 10 * iterations; nl++) {
        prod = 1.0f;
        
        // Vectorized chunk processing
        v8f32 vec_prod[CHUNK_SIZE / 8];
        for (int j = 0; j < CHUNK_SIZE / 8; j++) {
            vec_prod[j] = (v8f32){1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
        }
        
        int i = 0;
        for (; i + CHUNK_SIZE <= LEN_1D; i += CHUNK_SIZE) {
            // Process 8 elements at a time within each chunk
            for (int j = 0; j < CHUNK_SIZE / 8; j++) {
                v8f32 vec_a = *(v8f32*)&a[i + j * 8];
                vec_prod[j] *= vec_a;
            }
        }
        
        // Combine vector products
        float chunk_prod[CHUNK_SIZE];
        for (int j = 0; j < CHUNK_SIZE / 8; j++) {
            float* fp = (float*)&vec_prod[j];
            for (int k = 0; k < 8; k++) {
                chunk_prod[j * 8 + k] = fp[k];
            }
        }
        
        // Final reduction of chunk products
        for (int j = 0; j < CHUNK_SIZE; j++) {
            prod *= chunk_prod[j];
        }
        
        // Scalar tail
        for (; i < LEN_1D; i++) {
            prod *= a[i];
        }
    }
    return prod;
}
