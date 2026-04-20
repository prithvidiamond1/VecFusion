typedef float float4 __attribute__((ext_vector_type(4)));

float vectorized_s317(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;
    float factor = (float)0.99;

    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = (float)1.;
        float temp = q;

        // Vectorized multiplication
        int i = 0;
        if (half_len >= 4) {
            // Compute factor^4 for vector accumulation
            float4 factor_vec = (float4){factor, factor, factor, factor};
            float4 temp_vec = (float4){1.0f, 1.0f, 1.0f, 1.0f};
            
            // Unrolled vector accumulation
            for (; i <= half_len - 4; i += 4) {
                temp_vec *= factor_vec;
            }
            
            // Reduce vector to scalar
            temp = temp_vec[0] * temp_vec[1] * temp_vec[2] * temp_vec[3];
        }

        // Scalar tail
        for (; i < half_len; i++) {
            temp *= factor;
        }

        q = temp;
    }
    return q;
}
