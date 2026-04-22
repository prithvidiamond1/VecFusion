void s281_opt(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    int mid = LEN_1D / 2;
    
    for (int nl = 0; nl < iterations; nl++) {
        // First half: i from 0 to mid-1
        // Reads a[LEN_1D-i-1] where LEN_1D-i-1 >= i (unmodified a elements)
        for (int i = 0; i < mid; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
        
        // Second half: i from mid to LEN_1D-1
        // Reads a[LEN_1D-i-1] where LEN_1D-i-1 < i (already modified a elements)
        for (int i = mid; i < LEN_1D; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }
}
