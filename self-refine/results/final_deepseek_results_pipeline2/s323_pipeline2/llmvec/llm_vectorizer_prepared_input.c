void s323(int iterations,int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original b values for this iteration to break dependency
        float b_prev = b[0];
        
        // First compute all c*d and c*e products (independent computations)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = c[i] * d[i];  // Temporarily store c*d in a[i]
        }
        
        // Now compute the recurrence using stored products
        for (int i = 1; i < LEN_1D; i++) {
            float cd = a[i];  // Retrieve c[i]*d[i]
            float ce = c[i] * e[i];
            float a_new = b_prev + cd;
            b_prev = a_new + ce;  // This becomes b[i] for next iteration
            b[i] = b_prev;
            a[i] = a_new;
        }
    }
}
