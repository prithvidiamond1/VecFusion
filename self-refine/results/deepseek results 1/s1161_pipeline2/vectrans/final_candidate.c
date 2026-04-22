void s1161_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Process all i: compute both potential results
        for (int i = 0; i < LEN_1D-1; ++i) {
            float d_sq = d[i] * d[i];
            float de = d[i] * e[i];
            
            // Compute both branch results
            float a_new = c[i] + de;
            float b_new = a[i] + d_sq;
            
            // Mask-based selection without branching
            int cond = c[i] < 0.0f;
            a[i] = cond ? a[i] : a_new;      // Only update a if c[i] >= 0
            b[i] = cond ? b_new : b[i];      // Only update b if c[i] < 0
        }
    }
}
