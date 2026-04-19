void s442(int iterations,int LEN_1D, float* a, float *b,float* c, float *d, float* e, int* indx)
{
    int outer_iters = iterations/2;
    
    // Process each outer iteration separately to maintain aliasing semantics
    for (int nl = 0; nl < outer_iters; nl++) {
        // Split into 4 separate loops based on indx value to eliminate branching
        // Case 1: indx[i] == 1
        for (int i = 0; i < LEN_1D; i++) {
            if (indx[i] == 1) {
                a[i] += b[i] * b[i];
            }
        }
        
        // Case 2: indx[i] == 2
        for (int i = 0; i < LEN_1D; i++) {
            if (indx[i] == 2) {
                a[i] += c[i] * c[i];
            }
        }
        
        // Case 3: indx[i] == 3
        for (int i = 0; i < LEN_1D; i++) {
            if (indx[i] == 3) {
                a[i] += d[i] * d[i];
            }
        }
        
        // Case 4: indx[i] == 4
        for (int i = 0; i < LEN_1D; i++) {
            if (indx[i] == 4) {
                a[i] += e[i] * e[i];
            }
        }
    }
}