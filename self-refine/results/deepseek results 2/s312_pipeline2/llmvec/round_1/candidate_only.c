#include <stdint.h>

float vectorized_s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    for (int nl = 0; nl < 10 * iterations; nl++) {
        // Use 4-way unrolled accumulation to break serial dependency
        float p0 = 1.0f, p1 = 1.0f, p2 = 1.0f, p3 = 1.0f;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);

        for (; i < limit; i += 4) {
            p0 *= a[i + 0];
            p1 *= a[i + 1];
            p2 *= a[i + 2];
            p3 *= a[i + 3];
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            p0 *= a[i];
        }

        prod = p0 * p1 * p2 * p3;
    }
    return prod;
}
