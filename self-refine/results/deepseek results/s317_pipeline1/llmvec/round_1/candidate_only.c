float vectorized_s317(int iterations, int LEN_1D)
{
    float q;
    for (int nl = 0; nl < 5 * iterations; nl++) {
        // We want to compute 0.99^(LEN_1D/2)
        // Use 4-wide unrolling: maintain 4 accumulators, each computing
        // 0.99^(count/4) roughly, then combine at the end.
        int count = LEN_1D / 2;
        int vec_count = count / 4;
        int remainder = count - vec_count * 4;

        float q0 = 1.0f, q1 = 1.0f, q2 = 1.0f, q3 = 1.0f;
        for (int i = 0; i < vec_count; i++) {
            q0 *= 0.99f;
            q1 *= 0.99f;
            q2 *= 0.99f;
            q3 *= 0.99f;
        }
        q = q0 * q1 * q2 * q3;
        for (int i = 0; i < remainder; i++) {
            q *= 0.99f;
        }
    }
    return q;
}
