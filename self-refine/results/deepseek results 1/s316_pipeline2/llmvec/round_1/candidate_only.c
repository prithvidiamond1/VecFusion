#include <float.h>

float vectorized_s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        float min_val = a[0];

        // Unrolled reduction with 8-wide unrolling
        int i = 1;
        int limit = LEN_1D - 7;

        float m0, m1, m2, m3, m4, m5, m6, m7;
        m0 = m1 = m2 = m3 = m4 = m5 = m6 = m7 = min_val;

        for (; i < limit; i += 8) {
            float v0 = a[i + 0];
            float v1 = a[i + 1];
            float v2 = a[i + 2];
            float v3 = a[i + 3];
            float v4 = a[i + 4];
            float v5 = a[i + 5];
            float v6 = a[i + 6];
            float v7 = a[i + 7];

            if (v0 < m0) m0 = v0;
            if (v1 < m1) m1 = v1;
            if (v2 < m2) m2 = v2;
            if (v3 < m3) m3 = v3;
            if (v4 < m4) m4 = v4;
            if (v5 < m5) m5 = v5;
            if (v6 < m6) m6 = v6;
            if (v7 < m7) m7 = v7;
        }

        // Reduce the 8 accumulators
        if (m1 < m0) m0 = m1;
        if (m2 < m0) m0 = m2;
        if (m3 < m0) m0 = m3;
        if (m4 < m0) m0 = m4;
        if (m5 < m0) m0 = m5;
        if (m6 < m0) m0 = m6;
        if (m7 < m0) m0 = m7;

        min_val = m0;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float temp = a[i];
            if (temp < min_val) {
                min_val = temp;
            }
        }

        x = min_val;
    }
    return x;
}
