void s275_opt(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        // Compute bb[j][i]*cc[j][i] products into a temporary array
        float tmp[256][256];
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                tmp[j][i] = bb[j][i] * cc[j][i];
            }
        }

        // Store prev values for each column i
        float prev[256];
        float mask[256];
        for (int i = 0; i < 256; i++) {
            prev[i] = aa[0][i];
            mask[i] = (aa[0][i] > (float)0.) ? 1.0f : 0.0f;
        }

        // Outer loop over j, inner loop over i (vectorizable, no dependency across i)
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                prev[i] = prev[i] + tmp[j][i];
                aa[j][i] = aa[j][i] * (1.0f - mask[i]) + prev[i] * mask[i];
            }
        }
    }
}
