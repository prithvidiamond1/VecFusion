float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    float sum = 0.0f;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float sum_a = 0.0f;
        float sum_b = 0.0f;
        
        // First loop: compute a[i] and accumulate sum_a
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        v4f sum_a_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};
        
        // Vectorized part
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec = __builtin_convertvector(
                __builtin_memcpy_inline(
                    (char[16]){},
                    &c[i],
                    16
                ), v4f
            );
            v4f d_vec = __builtin_convertvector(
                __builtin_memcpy_inline(
                    (char[16]){},
                    &d[i],
                    16
                ), v4f
            );
            v4f tmp_a_vec = c_vec + d_vec;
            
            __builtin_memcpy_inline(&a[i], &tmp_a_vec, 16);
            sum_a_vec += tmp_a_vec;
        }
        
        // Reduce vector sum
        sum_a += ((float*)&sum_a_vec)[0] + ((float*)&sum_a_vec)[1] +
                 ((float*)&sum_a_vec)[2] + ((float*)&sum_a_vec)[3];
        
        // Scalar tail
        for (; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            a[i] = tmp_a;
            sum_a += tmp_a;
        }
        
        // Second loop: compute b[i] and accumulate sum_b
        i = 0;
        v4f sum_b_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};
        
        // Vectorized part
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec = __builtin_convertvector(
                __builtin_memcpy_inline(
                    (char[16]){},
                    &c[i],
                    16
                ), v4f
            );
            v4f e_vec = __builtin_convertvector(
                __builtin_memcpy_inline(
                    (char[16]){},
                    &e[i],
                    16
                ), v4f
            );
            v4f tmp_b_vec = c_vec + e_vec;
            
            __builtin_memcpy_inline(&b[i], &tmp_b_vec, 16);
            sum_b_vec += tmp_b_vec;
        }
        
        // Reduce vector sum
        sum_b += ((float*)&sum_b_vec)[0] + ((float*)&sum_b_vec)[1] +
                 ((float*)&sum_b_vec)[2] + ((float*)&sum_b_vec)[3];
        
        // Scalar tail
        for (; i < LEN_1D; i++) {
            float tmp_b = c[i] + e[i];
            b[i] = tmp_b;
            sum_b += tmp_b;
        }
        
        // Accumulate across outer iterations
        sum += sum_a + sum_b;
    }
    
    return sum;
}
