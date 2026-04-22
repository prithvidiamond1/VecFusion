typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    const int N = 256;
    const int VEC_SIZE = 4;
    const int VEC_ITERS = N / VEC_SIZE;
    const int REMAINDER = N % VEC_SIZE;

    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        float s_vals[N];

        // First loop: compute s values with cross-chunk dependency
        for (int i = 0; i < VEC_ITERS; i++) {
            int base = i * VEC_SIZE;
            float4 a_vec = *(float4*)&a[base];
            float4 d_vec = *(float4*)&d[base];

            // Vector comparison: a[i] > 0
            int4 mask = a_vec > (float4){0.0f, 0.0f, 0.0f, 0.0f};

            // Process elements sequentially to maintain dependency
            for (int j = 0; j < VEC_SIZE; j++) {
                if (mask[j]) {
                    s = d[base + j] * d[base + j];
                }
                s_vals[base + j] = s;
            }
        }

        // Handle remainder in first loop
        int base = VEC_ITERS * VEC_SIZE;
        for (int i = 0; i < REMAINDER; i++) {
            int idx = base + i;
            if (a[idx] > 0.0f) {
                s = d[idx] * d[idx];
            }
            s_vals[idx] = s;
        }

        // Second loop: vectorized computation b[i] = s_vals[i] * c[i] + d[i]
        for (int i = 0; i < VEC_ITERS; i++) {
            int base = i * VEC_SIZE;
            float4 s_vec = *(float4*)&s_vals[base];
            float4 c_vec = *(float4*)&c[base];
            float4 d_vec = *(float4*)&d[base];

            float4 b_vec = s_vec * c_vec + d_vec;
            *(float4*)&b[base] = b_vec;
        }

        // Handle remainder in second loop
        base = VEC_ITERS * VEC_SIZE;
        for (int i = 0; i < REMAINDER; i++) {
            int idx = base + i;
            b[idx] = s_vals[idx] * c[idx] + d[idx];
        }

        // Third loop: vectorized computation e[i] = (s_vals[i] + 1.0f) * aa[0][i]
        for (int i = 0; i < VEC_ITERS; i++) {
            int base = i * VEC_SIZE;
            float4 s_vec = *(float4*)&s_vals[base];
            float4 aa_vec = *(float4*)&aa[0][base];

            float4 e_vec = (s_vec + (float4){1.0f, 1.0f, 1.0f, 1.0f}) * aa_vec;
            *(float4*)&e[base] = e_vec;
        }

        // Handle remainder in third loop
        base = VEC_ITERS * VEC_SIZE;
        for (int i = 0; i < REMAINDER; i++) {
            int idx = base + i;
            e[idx] = (s_vals[idx] + 1.0f) * aa[0][idx];
        }
    }
}
