void s291(int iterations,int LEN_1D, float* a,float* b)
{
    int im1;
    float prev_b;
    
    for (int nl = 0; nl < 2*iterations; nl++) {
        im1 = LEN_1D-1;
        prev_b = b[im1];
        
        for (int i = 0; i < LEN_1D; i++) {
            float current_b = b[i];
            a[i] = (current_b + prev_b) * (float).5;
            prev_b = current_b;
        }
    }
}
