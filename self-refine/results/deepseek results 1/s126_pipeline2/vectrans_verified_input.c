void s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = 1;
        // Process each column independently
        for (int i = 0; i < 256; i++) {
            // Compute all products for this column first
            float products[256];
            for (int j = 1; j < 256; j++) {
                products[j] = flat_2d_array[k-1] * cc[j][i];
                k++;
            }
            k++; // Extra increment after each column
            
            // Now update bb column with simple forward dependency
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j-1][i] + products[j];
            }
        }
    }
}