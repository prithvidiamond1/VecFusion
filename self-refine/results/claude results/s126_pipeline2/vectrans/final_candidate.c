void s126_opt(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                bb[j][i] = bb[j-1][i] + flat_2d_array[i*256 + (j-1)] * cc[j][i];
            }
        }
    }
}
