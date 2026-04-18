#include <stdint.h>

void vectorized_s141(int k, int i, int k_offsets[256][256]) {
    int base_k = k;
    k = base_k;
    for (int j = i; j < 256; j++) {
        k_offsets[i][j] = k;
        k += j + 1;
    }
}
