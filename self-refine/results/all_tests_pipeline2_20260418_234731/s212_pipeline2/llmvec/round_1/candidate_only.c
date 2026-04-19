#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef float v8f __attribute__((__vector_size__(32), __aligned__(32)));

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    if (n <= 1) return;
    
    // Allocate buffer for saved a[i+1] values
    float *a_next = (float *)malloc((n-1) * sizeof(float));
    if (a_next == NULL) return;
    
    int i = 0;
    const int vlen = 8;
    int limit = (n-1) - (n-1) % vlen;
    
    // First loop: compute a[i] *= c[i] and save original a[i+1]
    for (; i < limit; i += vlen) {
        // Load a[i] and c[i] for vector multiplication
        v8f a_vec = *(v8f *)&a[i];
        v8f c_vec = *(v8f *)&c[i];
        
        // Compute a[i] *= c[i]
        a_vec = a_vec * c_vec;
        
        // Store result back to a[i]
        *(v8f *)&a[i] = a_vec;
        
        // Load a[i+1] values for saving
        v8f a_next_vec = *(v8f *)&a[i+1];
        
        // Store saved values to a_next[i]
        *(v8f *)&a_next[i] = a_next_vec;
    }
    
    // Scalar tail for first loop
    for (; i < n-1; i++) {
        a_next[i] = a[i+1];
        a[i] *= c[i];
    }
    
    // Second loop: compute b[i] += saved_a_next * d[i]
    i = 0;
    limit = (n-1) - (n-1) % vlen;
    
    for (; i < limit; i += vlen) {
        // Load b[i], a_next[i], and d[i]
        v8f b_vec = *(v8f *)&b[i];
        v8f a_next_vec = *(v8f *)&a_next[i];
        v8f d_vec = *(v8f *)&d[i];
        
        // Compute b[i] += a_next[i] * d[i]
        b_vec = b_vec + a_next_vec * d_vec;
        
        // Store result back to b[i]
        *(v8f *)&b[i] = b_vec;
    }
    
    // Scalar tail for second loop
    for (; i < n-1; i++) {
        b[i] += a_next[i] * d[i];
    }
    
    free(a_next);
}
