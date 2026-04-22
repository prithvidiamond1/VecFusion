#include <stddef.h>

typedef float float32_t __attribute__((ext_vector_type(4)));

void vectorized_s453(int iterations, int LEN_1D, float* a, float *b) {
    float s;
    int total_iters = iterations * 2;
    for (int nl = 0; nl < total_iters; nl++) {
        s = 0.0f;
        int i = 0;
        // Vectorized main loop
        for (; i + 4 <= LEN_1D; i += 4) {
            float32_t s_vec = {s, s + 1.0f, s + 2.0f, s + 3.0f};
            float32_t b_vec = *(float32_t*)(b + i);
            float32_t a_vec = s_vec * b_vec;
            *(float32_t*)(a + i) = a_vec;
            s += 4.0f;
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            s += 1.0f;
            a[i] = s * b[i];
        }
    }
}
