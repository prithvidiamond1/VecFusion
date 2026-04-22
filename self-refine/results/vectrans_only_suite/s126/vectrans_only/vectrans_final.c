
void s126_opt(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = 1;
        // Precompute k values for each (j,i) position to enable vectorization
        for (int j = 1; j < 256; j++) {
            // Vectorizable loop across i dimension
            for (int i = 0; i < 256; i++) {
                // Calculate the exact k index for this position
                int k_index = (j - 1) + 256 * i;
                bb[j][i] = bb[j-1][i] + flat_2d_array[k_index] * cc[j][i];
            }
        }
    }
}
