void s2111(int iterations, float aa[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        // Wavefront (diagonal) traversal to eliminate loop-carried dependencies
        // Elements on the same anti-diagonal d = i + j have no dependencies on each other
        for (int d = 2; d <= 510; d++) {
            int j_start = (d - 255 > 1) ? d - 255 : 1;
            int j_end   = (d - 1 < 255) ? d - 1 : 255;
            for (int j = j_start; j <= j_end; j++) {
                int i = d - j;
                aa[j][i] = (aa[j][i-1] + aa[j-1][i]) / 1.9f;
            }
        }
    }
}