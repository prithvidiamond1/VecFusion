float s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    
    for (int nl = 0; nl < 10*iterations; nl++) {
        prod = (float)1.;
        
        // Loop distribution - separate the reduction into independent chunks
        const int CHUNK_SIZE = 64;  // Cache-friendly chunk size
        float chunk_prod[CHUNK_SIZE];
        
        // Initialize chunk products
        for (int j = 0; j < CHUNK_SIZE; j++) {
            chunk_prod[j] = (float)1.;
        }
        
        // Process in chunks to break the long dependency chain
        int i = 0;
        for (; i + CHUNK_SIZE <= LEN_1D; i += CHUNK_SIZE) {
            // Process each chunk with independent reduction
            for (int j = 0; j < CHUNK_SIZE; j++) {
                chunk_prod[j] *= a[i + j];
            }
        }
        
        // Combine chunk products (preserving original order)
        for (int j = 0; j < CHUNK_SIZE; j++) {
            prod *= chunk_prod[j];
        }
        
        // Handle remaining elements sequentially
        for (; i < LEN_1D; i++) {
            prod *= a[i];
        }
    }
    return prod;
}