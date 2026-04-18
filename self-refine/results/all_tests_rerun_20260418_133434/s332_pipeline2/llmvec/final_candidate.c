#include <stdint.h>
#include <float.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a)
{
    int index;
    float value;
    float chksum;

    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;

        int first_index = LEN_1D;

        // Vectorized portion: process 8 elements at a time
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);

        for (; i < limit; i += 8) {
            // Unrolled 8-wide SIMD-friendly min-index reduction
            int c0 = (a[i+0] > t) ? (i+0) : LEN_1D;
            int c1 = (a[i+1] > t) ? (i+1) : LEN_1D;
            int c2 = (a[i+2] > t) ? (i+2) : LEN_1D;
            int c3 = (a[i+3] > t) ? (i+3) : LEN_1D;
            int c4 = (a[i+4] > t) ? (i+4) : LEN_1D;
            int c5 = (a[i+5] > t) ? (i+5) : LEN_1D;
            int c6 = (a[i+6] > t) ? (i+6) : LEN_1D;
            int c7 = (a[i+7] > t) ? (i+7) : LEN_1D;

            // Reduce within the 8 elements
            int m01 = (c0 < c1) ? c0 : c1;
            int m23 = (c2 < c3) ? c2 : c3;
            int m45 = (c4 < c5) ? c4 : c5;
            int m67 = (c6 < c7) ? c6 : c7;
            int m0123 = (m01 < m23) ? m01 : m23;
            int m4567 = (m45 < m67) ? m45 : m67;
            int mlocal = (m0123 < m4567) ? m0123 : m4567;

            if (mlocal < first_index)
                first_index = mlocal;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            int candidate = (a[i] > t) ? i : LEN_1D;
            if (candidate < first_index)
                first_index = candidate;
        }

        // Sequential extraction
        if (first_index < LEN_1D) {
            index = first_index;
            value = a[first_index];
        }

        chksum = value + (float)index;
    }
    return value;
}
