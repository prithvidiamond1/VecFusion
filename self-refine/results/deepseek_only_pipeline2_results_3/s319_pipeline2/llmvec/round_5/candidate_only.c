float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    float sum = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        float sum_a = 0.0f;
        float sum_b = 0.0f;

        // First loop: compute a[i] = c[i] + d[i] and accumulate sum_a
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        v4f sum_a_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};

        // Vectorized part for a[i]
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec, d_vec;
            __builtin_memcpy(&c_vec, &c[i], 16);
            __builtin_memcpy(&d_vec, &d[i], 16);
            v4f tmp_a_vec = c_vec + d_vec;

            __builtin_memcpy(&a[i], &tmp_a_vec, 16);
            sum_a_vec += tmp_a_vec;
        }

        // Reduce vector sum for sum_a
        float sum_a_vec_arr[4];
        __builtin_memcpy(sum_a_vec_arr, &sum_a_vec, 16);
        sum_a = sum_a_vec_arr[0] + sum_a_vec_arr[1] + sum_a_vec_arr[2] + sum_a_vec_arr[3];

        // Scalar tail for a[i]
        for (; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            a[i] = tmp_a;
            sum_a += tmp_a;
        }

        // Second loop: compute b[i] = c[i] + (c[i] < 0 ? d[i] : e[i]) and accumulate sum_b
        i = 0;
        v4f sum_b_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};

        // Vectorized part for b[i] with conditional selection
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec, d_vec, e_vec;
            __builtin_memcpy(&c_vec, &c[i], 16);
            __builtin_memcpy(&d_vec, &d[i], 16);
            __builtin_memcpy(&e_vec, &e[i], 16);

            // Create mask for c_vec < 0
            v4f mask = __builtin_convertvector(c_vec < (v4f){0.0f, 0.0f, 0.0f, 0.0f}, v4f);
            
            // Select between d_vec and e_vec based on mask
            v4f selected_vec = (mask != (v4f){0.0f, 0.0f, 0.0f, 0.0f}) ? d_vec : e_vec;
            
            // Compute b[i] = c[i] + selected_vec
            v4f tmp_b_vec = c_vec + selected_vec;

            __builtin_memcpy(&b[i], &tmp_b_vec, 16);
            sum_b_vec += tmp_b_vec;
        }

        // Reduce vector sum for sum_b
        float sum_b_vec_arr[4];
        __builtin_memcpy(sum_b_vec_arr, &sum_b_vec, 16);
        sum_b = sum_b_vec_arr[0] + sum_b_vec_arr[1] + sum_b_vec_arr[2] + sum_b_vec_arr[3];

        // Scalar tail for b[i] with conditional
        for (; i < LEN_1D; i++) {
            float selected = (c[i] < 0.0f) ? d[i] : e[i];
            float tmp_b = c[i] + selected;
            b[i] = tmp_b;
            sum_b += tmp_b;
        }

        // Accumulate across outer iterations
        sum += sum_a + sum_b;
    }

    return sum;
}
