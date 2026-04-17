#include <stdint.h>

void vectorized_s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        // First loop: aa[j][i] = aa[j-1][i] + cc[j][i]
        // For each column i (1..255), iterate over rows j (1..255)
        // Each j depends on j-1, so we vectorize over i (columns)
        for (int i = 1; i < 256; i++) {
            // Vectorize over j with unrolling - but j has dependency on j-1
            // Actually j loop has dependency: aa[j][i] depends on aa[j-1][i]
            // So we must keep j as outer sequential loop, vectorize i
            // Wait - original has i outer, j inner with dependency on j-1
            // Let's keep it sequential as written but unroll i
        }
        
        // First loop: outer i, inner j with j-dependency - must be sequential in j
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }

        // Second loop: bb[j][i] = bb[j][i-1] + cc[j][i]
        // outer j, inner i with i-dependency - must be sequential in i
        // But we can vectorize over j (outer loop) by unrolling
        // Each j row is independent of other j rows
        // Unroll j loop by 4
        int j = 1;
        for (; j <= 252; j += 4) {
            for (int i = 1; i < 256; i++) {
                bb[j+0][i] = bb[j+0][i-1] + cc[j+0][i];
                bb[j+1][i] = bb[j+1][i-1] + cc[j+1][i];
                bb[j+2][i] = bb[j+2][i-1] + cc[j+2][i];
                bb[j+3][i] = bb[j+3][i-1] + cc[j+3][i];
            }
        }
        for (; j < 256; j++) {
            for (int i = 1; i < 256; i++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
    }
}
