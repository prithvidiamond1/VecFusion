#include <stdint.h>

void vectorized_s222(int iterations, int LEN_1D, float*a, float*b, float*c, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // The e[i] = e[i-1] * e[i-1] loop has a loop-carried dependency,
        // so it must remain scalar. However, a[i] += b[i]*c[i]; a[i] -= b[i]*c[i]
        // simplifies to a[i] = a[i] (no net change), but we must preserve semantics.
        // We'll vectorize the a[] updates where possible and keep e[] scalar.

        // Note: a[i] += b[i]*c[i] followed by a[i] -= b[i]*c[i] is a no-op for a[],
        // but we preserve it for correctness (side effects, floating point).
        // The e[] loop has a serial dependency chain.

        int i = 1;

        // Vectorize a[] updates in chunks of 4, then handle e[] serially
        // Actually since a[i] += bc then a[i] -= bc, net effect on a is zero
        // but we must still do it for floating point exactness preservation.
        // We'll do both loops: vectorize a[] part, scalar e[] part.

        // Process a[] vectorized (no dependency)
        int vec_end = 1 + ((LEN_1D - 1) / 4) * 4;
        if (vec_end > LEN_1D) vec_end = LEN_1D;

        for (i = 1; i + 3 < LEN_1D; i += 4) {
            float bc0 = b[i+0] * c[i+0];
            float bc1 = b[i+1] * c[i+1];
            float bc2 = b[i+2] * c[i+2];
            float bc3 = b[i+3] * c[i+3];
            a[i+0] += bc0;
            a[i+1] += bc1;
            a[i+2] += bc2;
            a[i+3] += bc3;
            a[i+0] -= bc0;
            a[i+1] -= bc1;
            a[i+2] -= bc2;
            a[i+3] -= bc3;
        }
        // scalar tail for a[]
        for (; i < LEN_1D; i++) {
            float bc = b[i] * c[i];
            a[i] += bc;
            a[i] -= bc;
        }

        // e[] has loop-carried dependency: e[i] = e[i-1] * e[i-1], must be scalar
        for (i = 1; i < LEN_1D; i++) {
            e[i] = e[i-1] * e[i-1];
        }
    }
}
