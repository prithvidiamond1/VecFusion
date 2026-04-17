void s235_opt(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 200*(iterations/256); nl++) {
        // First loop: update a[i] - vectorizable
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }
        // Second loop: compute aa[j][i] using updated a[i]
        // The j-loop has loop-carried dependency (aa[j][i] depends on aa[j-1][i])
        // so we keep j as outer loop sequential, i as inner loop vectorizable
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
            }
        }
    }
}
