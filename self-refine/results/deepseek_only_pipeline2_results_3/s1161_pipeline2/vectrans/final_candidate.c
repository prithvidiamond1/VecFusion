void s1161_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute values for both branches into temporaries
        for (int i = 0; i < LEN_1D-1; ++i) {
            float temp_a = c[i] + d[i] * e[i];
            float temp_b = a[i] + d[i] * d[i];
            
            // Store results based on condition
            if (c[i] < (float)0.) {
                b[i] = temp_b;
            } else {
                a[i] = temp_a;
            }
        }
    }
}
