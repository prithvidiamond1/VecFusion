#include <stddef.h>

void vectorized_s244(int n, float *a, float *b, float *c, float *d) {
    // The loop has a carried dependency: b[i] is written then read for a[i+1]
    // which depends on b[i] (the new value). We process pairs of iterations
    // since a[i+1] depends on b[i] (same iteration pair).
    // Original scalar loop (assumed 1-indexed, stride 1):
    // for i in 0..n-1 (or similar):
    //   a[i]   = b[i] + c[i] * d[i]
    //   b[i]   = c[i] + b[i]
    //   a[i+1] = b[i] + a[i+1] * d[i]
    //
    // Note: a[i+1] is written here and a[i+1] will be read as a[i] in next iteration
    // but a[i] = b[i] + c[i]*d[i] uses the OLD a[i], so there's no read-after-write
    // on a[] across iterations. b[i] is only written once per i, so b[] has no
    // cross-iteration dependency either.
    // We can vectorize by processing chunks, computing all three statements in order.

    typedef float v4f __attribute__((vector_size(16)));

    int i = 0;
    int limit = n - 3;

    for (; i <= limit; i += 4) {
        v4f bi, ci, di, ai, ai1;

        // Load
        __builtin_memcpy(&bi,  b + i, 16);
        __builtin_memcpy(&ci,  c + i, 16);
        __builtin_memcpy(&di,  d + i, 16);
        __builtin_memcpy(&ai,  a + i, 16);
        __builtin_memcpy(&ai1, a + i + 1, 16);

        // a[i] = b[i] + c[i] * d[i]
        v4f new_ai = bi + ci * di;

        // b[i] = c[i] + b[i]  (new b[i])
        v4f new_bi = ci + bi;

        // a[i+1] = b[i](new) + a[i+1] * d[i]
        v4f new_ai1 = new_bi + ai1 * di;

        // Store results
        __builtin_memcpy(a + i,     &new_ai,  16);
        __builtin_memcpy(b + i,     &new_bi,  16);
        __builtin_memcpy(a + i + 1, &new_ai1, 16);
    }

    // Scalar cleanup
    for (; i < n; i++) {
        a[i]   = b[i] + c[i] * d[i];
        b[i]   = c[i] + b[i];
        a[i+1] = b[i] + a[i+1] * d[i];
    }
}
