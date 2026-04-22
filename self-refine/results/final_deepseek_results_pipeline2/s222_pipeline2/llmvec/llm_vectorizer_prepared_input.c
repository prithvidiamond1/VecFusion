void s222(int iterations, int LEN_1D, float*a,float*b,float*c, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Split the loop: compute e[i] first (has loop-carried dependency)
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }
        // Then compute a[i] updates (no loop-carried dependency, vectorizable)
        for (int i = 1; i < LEN_1D; i++) {
            float temp_bc = b[i] * c[i];
            a[i] += temp_bc;
            a[i] -= temp_bc;
        }
    }
}
