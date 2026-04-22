void s126_opt(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = 1;
        for (int i = 0; i < 256; i++) {
            float temp_bb = bb[0][i];
            for (int j = 1; j < 256; j++) {
                float flat_val = flat_2d_array[k-1];
                temp_bb = temp_bb + flat_val * cc[j][i];
                bb[j][i] = temp_bb;
                ++k;
            }
            ++k;
        }
    }
}
