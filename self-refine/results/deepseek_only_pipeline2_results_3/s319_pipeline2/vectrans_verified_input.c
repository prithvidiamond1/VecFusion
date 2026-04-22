float s319(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e) {
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        float sum_a = 0.;
        float sum_b = 0.;
        
        // First loop: compute a[i] and accumulate its sum
        for (int i = 0; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            a[i] = tmp_a;
            sum_a += tmp_a;
        }
        
        // Second loop: compute b[i] and accumulate its sum
        for (int i = 0; i < LEN_1D; i++) {
            float tmp_b = c[i] + e[i];
            b[i] = tmp_b;
            sum_b += tmp_b;
        }
        
        // Combine the partial sums
        sum = sum_a + sum_b;
    }
    return sum;
}