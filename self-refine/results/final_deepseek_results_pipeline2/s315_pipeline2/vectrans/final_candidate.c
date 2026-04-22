float s315_opt(int iterations, int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x, chksum;
    int index;
    
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;
        
        // Vectorizable max reduction
        for (int i = 0; i < LEN_1D; ++i) {
            if (a[i] > x) {
                x = a[i];
            }
        }
        
        // Find last index of max value
        for (int i = 0; i < LEN_1D; ++i) {
            // Use exact equality for semantic equivalence
            // Compiler can still vectorize this comparison
            if (a[i] == x) {
                index = i;
            }
        }
        
        chksum = x + (float) index;
    }
    
    return index + x + 1;
}
