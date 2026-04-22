
void s256_opt(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    for (int nl = 0; nl < 10 * (iterations / 256); nl++) {
        // Solve recurrence: a[j] = 1 - a[j-1] with a[0] fixed
        float a0 = a[0];
        float even_val = a0;
        float odd_val = 1.0f - a0;
        
        // Vector-friendly initialization of a_temp
        float a_temp[256];
        a_temp[0] = a0;
        for (int j = 1; j < 256; j++) {
            a_temp[j] = (j & 1) ? odd_val : even_val;
        }
        
        // Copy back to original array
        for (int j = 0; j < 256; j++) {
            a[j] = a_temp[j];
        }
        
        // Loop distribution: separate j=0 case if needed (here j starts at 1)
        // Interchange loops for vectorization
        for (int j = 1; j < 256; j++) {
            float a_val = a_temp[j];
            float d_val = d[j];
            // This inner loop is fully vectorizable
            for (int i = 0; i < 256; i++) {
                aa[j][i] = a_val + bb[j][i] * d_val;
            }
        }
    }
}
