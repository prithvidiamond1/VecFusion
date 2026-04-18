float s312_opt(int iterations, int LEN_1D, float* a)
{
    float prod;
    for (int nl = 0; nl < 10*iterations; nl++) {
        prod = (float)1.;
        // Loop distribution: separate reduction from array access
        // This allows the compiler to vectorize the product accumulation
        for (int i = 0; i < LEN_1D; i++) {
            prod = prod * a[i];
        }
    }
    return prod;
}
