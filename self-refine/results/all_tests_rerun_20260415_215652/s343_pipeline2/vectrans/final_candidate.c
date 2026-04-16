void s343_opt(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int cond[256][256];
    int row_count[256];
    int row_start[257];

    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        // Compute conditions: cond[j][i] = bb[j][i] > 0, j-outer for contiguous bb access
        for (int j = 0; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                cond[j][i] = (bb[j][i] > (float)0.);
            }
        }

        // Compute per-row counts (vectorizable reduction)
        for (int j = 0; j < 256; j++) {
            int cnt = 0;
            for (int i = 0; i < 256; i++) {
                cnt += cond[j][i];
            }
            row_count[j] = cnt;
        }

        // Compute row start offsets (sequential prefix sum, only 256 iterations)
        row_start[0] = 0;
        for (int j = 0; j < 256; j++) {
            row_start[j+1] = row_start[j] + row_count[j];
        }

        // Scatter values using per-row local offset (each row independent)
        for (int j = 0; j < 256; j++) {
            int base = row_start[j];
            for (int i = 0; i < 256; i++) {
                flat_2d_array[base] = aa[j][i];
                base += cond[j][i];
            }
        }
    }
}
