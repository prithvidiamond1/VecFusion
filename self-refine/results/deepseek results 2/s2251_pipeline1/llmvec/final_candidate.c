void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        int i = 0;
        
        // Unrolled by 4 - the s recurrence is serial, but we can pipeline
        // the independent a[i] and b[i] computations
        for (; i <= LEN_1D - 4; i += 4) {
            float s0, s1, s2, s3;
            float a0, a1, a2, a3;

            // Iteration i+0
            a0 = s * e[i+0];
            s0 = b[i+0] + c[i+0];
            
            // Iteration i+1
            a1 = s0 * e[i+1];
            s1 = b[i+1] + c[i+1];
            
            // Iteration i+2
            a2 = s1 * e[i+2];
            s2 = b[i+2] + c[i+2];
            
            // Iteration i+3
            a3 = s2 * e[i+3];
            s3 = b[i+3] + c[i+3];

            // Now write back a and b (these are independent of each other)
            a[i+0] = a0;
            a[i+1] = a1;
            a[i+2] = a2;
            a[i+3] = a3;

            b[i+0] = a0 + d[i+0];
            b[i+1] = a1 + d[i+1];
            b[i+2] = a2 + d[i+2];
            b[i+3] = a3 + d[i+3];

            s = s3;
        }
        
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] = s * e[i];
            s = b[i] + c[i];
            b[i] = a[i] + d[i];
        }
    }
}
