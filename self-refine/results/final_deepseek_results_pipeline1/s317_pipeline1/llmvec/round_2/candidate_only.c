typedef float float32 __attribute__((ext_vector_type(4)));

float vectorized_s317(int iterations, int LEN_1D) {
    float q = 1.0f;
    int total_outer = 5 * iterations;
    int inner_len = LEN_1D;

    // Vectorized inner loop: process 4 iterations at a time
    float32 vq = {1.0f, 1.0f, 1.0f, 1.0f};
    float32 vfactor = {0.99f, 0.99f, 0.99f, 0.99f};
    int i = 0;
    for (; i + 4 <= inner_len; i += 4) {
        vq *= vfactor;
    }
    // Reduce vector lanes to a single scalar
    float scalar_q = vq[0] * vq[1] * vq[2] * vq[3];
    // Scalar cleanup for remaining inner iterations
    for (; i < inner_len; i++) {
        scalar_q *= 0.99f;
    }

    // Outer loop: multiply q by scalar_q for each outer iteration
    for (int nl = 0; nl < total_outer; nl++) {
        q *= scalar_q;
    }

    return q;
}
