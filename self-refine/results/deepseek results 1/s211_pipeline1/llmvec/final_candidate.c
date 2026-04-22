#include <stdint.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Note: b[i] depends on b[i+1] and b[i-1] in the same iteration,
        // but since a[i] uses b[i-1] (old value) and b[i] uses b[i+1] (old value),
        // and b[i] is written after a[i] is computed, we need to be careful.
        // The loop reads b[i-1] for a[i] and b[i+1] for b[i].
        // Since b[i] is updated in order i=1,2,..., b[i+1] hasn't been updated yet
        // when we compute b[i], so we can vectorize if we save old b values first.
        
        int len = LEN_1D - 2; // number of iterations: i from 1 to LEN_1D-2
        if (len <= 0) continue;
        
        // We need old b values since b is both read and written.
        // b[i] reads b[i-1] (already written if i>1 in scalar) and b[i+1] (not yet written).
        // Wait - in scalar: a[i] = b[i-1] + c[i]*d[i], then b[i] = b[i+1] - e[i]*d[i]
        // When computing i=2: a[2] = b[1] (already updated at i=1) + c[2]*d[2]
        // So there IS a loop-carried dependency through b[i-1] for a[i].
        // This means we cannot simply vectorize without saving old b.
        // We'll copy b to a temp buffer first, then vectorize.
        
        // Use a local temp copy of b for the old values
        // To avoid VLA issues with large sizes, we'll just do scalar with unrolling
        // but structured for vectorization where possible.
        
        // Since b[i-1] used in a[i] depends on b updated at i-1, we must use
        // a temporary copy of b. Let's do it with a simple approach:
        // save b[1..LEN_1D-2] old values, then vectorize.
        
        // Allocate temp on stack only if small, otherwise just do scalar unroll
        // For correctness, let's just do 4-wide unroll with scalar semantics preserved
        // by processing sequentially (the dependency prevents true SIMD here for a[i]).
        
        // Actually let's just unroll 4x scalar - the b[i-1] dependency makes true
        // vectorization of a[i] incorrect without a temp copy.
        
        int i = 1;
        for (; i <= LEN_1D - 5; i += 4) {
            float a0 = b[i-1]   + c[i]   * d[i];
            float b0 = b[i+1]   - e[i]   * d[i];
            float a1 = b0       + c[i+1] * d[i+1];  // b[i] = b0 (just updated)
            float b1 = b[i+2]   - e[i+1] * d[i+1];
            float a2 = b1       + c[i+2] * d[i+2];
            float b2 = b[i+3]   - e[i+2] * d[i+2];
            float a3 = b2       + c[i+3] * d[i+3];
            float b3 = b[i+4]   - e[i+3] * d[i+3];
            a[i]   = a0; b[i]   = b0;
            a[i+1] = a1; b[i+1] = b1;
            a[i+2] = a2; b[i+2] = b2;
            a[i+3] = a3; b[i+3] = b3;
        }
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
            b[i] = b[i+1] - e[i] * d[i];
        }
    }
}
