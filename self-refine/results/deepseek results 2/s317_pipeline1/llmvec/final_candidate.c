float vectorized_s317(int iterations, int LEN_1D)
{
    float q;
    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = (float)1.;
        int limit = LEN_1D / 2;

        // Use 4-wide unrolling to allow vectorization
        // Each "lane" accumulates independently, then we combine at the end
        float q0 = 1.0f, q1 = 1.0f, q2 = 1.0f, q3 = 1.0f;
        int i = 0;
        int limit4 = limit - (limit % 4);
        for (; i < limit4; i += 4) {
            q0 *= 0.99f;
            q1 *= 0.99f;
            q2 *= 0.99f;
            q3 *= 0.99f;
        }
        // Combine lanes
        float combined = q0 * q1 * q2 * q3;
        // Scalar tail
        for (; i < limit; i++) {
            combined *= 0.99f;
        }
        q = combined;
    }
    return q;
}
