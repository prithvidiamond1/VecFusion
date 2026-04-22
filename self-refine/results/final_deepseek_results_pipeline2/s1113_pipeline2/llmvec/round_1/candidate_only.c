typedef float v8f __attribute__((__vector_size__(32), __aligned__(4)));

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;
    int outer_iterations = 2 * iterations;
    
    for (int nl = 0; nl < outer_iterations; nl++) {
        float a_mid_original = a[mid];
        float updated_mid = a_mid_original + b[mid];
        
        // Vectorize first loop: i = 0 to mid-1
        int i = 0;
        int first_loop_bound = mid;
        v8f a_mid_vec = (v8f){a_mid_original, a_mid_original, a_mid_original, a_mid_original,
                              a_mid_original, a_mid_original, a_mid_original, a_mid_original};
        
        for (; i + 7 < first_loop_bound; i += 8) {
            v8f b_vec = *(v8f*)&b[i];
            v8f result_vec = a_mid_vec + b_vec;
            *(v8f*)&a[i] = result_vec;
        }
        
        // Scalar tail for first loop
        for (; i < first_loop_bound; i++) {
            a[i] = a_mid_original + b[i];
        }
        
        // Vectorize second loop: i = mid+1 to LEN_1D-1
        i = mid + 1;
        int second_loop_bound = LEN_1D;
        v8f updated_mid_vec = (v8f){updated_mid, updated_mid, updated_mid, updated_mid,
                                    updated_mid, updated_mid, updated_mid, updated_mid};
        
        for (; i + 7 < second_loop_bound; i += 8) {
            v8f b_vec = *(v8f*)&b[i];
            v8f result_vec = updated_mid_vec + b_vec;
            *(v8f*)&a[i] = result_vec;
        }
        
        // Scalar tail for second loop
        for (; i < second_loop_bound; i++) {
            a[i] = updated_mid + b[i];
        }
        
        a[mid] = updated_mid;
    }
}
