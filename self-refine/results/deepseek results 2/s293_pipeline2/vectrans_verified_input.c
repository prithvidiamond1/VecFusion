void s293(int iterations,int LEN_1D, float* a)
{
    float temp = a[0];
    int total_outer = 4 * iterations;
    
    for (int nl = 0; nl < total_outer; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = temp;
        }
    }
}