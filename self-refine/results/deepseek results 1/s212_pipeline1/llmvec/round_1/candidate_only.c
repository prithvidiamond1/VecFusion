#include <stdint.h>

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    // The loop has a dependency: a[i] is written, but a[i+1] is read.
    // a[i] *= c[i] writes a[i], and b[i] += a[i+1] * d[i] reads a[i+1].
    // Since we write a[i] and read a[i+1], these are different indices,
    // so there is no loop-carried dependency on a[] for the b[] update
    // (the read of a[i+1] uses the UPDATED value from the previous iteration's write).
    // Wait: iteration i writes a[i] and reads a[i+1].
    // iteration i+1 writes a[i+1] and reads a[i+2].
    // So b[i] += a[i+1]*d[i] uses the ORIGINAL a[i+1] (not yet updated by iteration i+1).
    // This means we need to be careful: we must read a[i+1] before updating a[i+1].
    // 
    // Strategy: preload a[i+1] values before updating a[i], then use them for b[i].
    // We can vectorize by:
    // 1. Load a[1..n-1] (the a[i+1] values) into a temp array or registers
    // 2. Update a[0..n-2] *= c[0..n-2]
    // 3. Update b[0..n-2] += temp * d[0..n-2]

    int limit = n - 1;
    if (limit <= 0) return;

    typedef float float4 __attribute__((vector_size(16)));

    int i = 0;
    int vec_limit = limit - (limit % 4);

    for (; i < vec_limit; i += 4) {
        // Load a[i+1], a[i+2], a[i+3], a[i+4] before modifying a[i..i+3]
        float4 a_next;
        a_next[0] = a[i + 1];
        a_next[1] = a[i + 2];
        a_next[2] = a[i + 3];
        a_next[3] = a[i + 4];

        float4 ai, ci, bi, di;
        ai[0] = a[i];     ai[1] = a[i+1]; ai[2] = a[i+2]; ai[3] = a[i+3];
        ci[0] = c[i];     ci[1] = c[i+1]; ci[2] = c[i+2]; ci[3] = c[i+3];
        bi[0] = b[i];     bi[1] = b[i+1]; bi[2] = b[i+2]; bi[3] = b[i+3];
        di[0] = d[i];     di[1] = d[i+1]; di[2] = d[i+2]; di[3] = d[i+3];

        // a[i] *= c[i]
        float4 ai_new = ai * ci;
        a[i]   = ai_new[0];
        a[i+1] = ai_new[1];
        a[i+2] = ai_new[2];
        a[i+3] = ai_new[3];

        // b[i] += a[i+1] * d[i]  (using original a[i+1] values)
        float4 bi_new = bi + a_next * di;
        b[i]   = bi_new[0];
        b[i+1] = bi_new[1];
        b[i+2] = bi_new[2];
        b[i+3] = bi_new[3];
    }

    // Scalar tail
    for (; i < limit; i++) {
        float a_next_val = a[i + 1];
        a[i] *= c[i];
        b[i] += a_next_val * d[i];
    }
}
