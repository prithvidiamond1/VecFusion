#include <stdint.h>

void vectorized_s126(float bb[256][256], float products[256], int i) {
    float prev_bb = bb[0][i];
    // This is a prefix sum (scan) loop — each iteration depends on the previous.
    // True sequential dependency: bb[j][i] = products[1] + products[2] + ... + products[j] + prev_bb
    // We can compute a prefix sum of products[1..255] and then add prev_bb to each element.

    // Step 1: compute prefix sums of products[1..255]
    float prefix[256];
    prefix[0] = products[1];
    for (int j = 1; j < 255; j++) {
        prefix[j] = prefix[j - 1] + products[j + 1];
    }

    // Step 2: add prev_bb to each prefix sum and store
    // This step is vectorizable
    int j;
    for (j = 0; j + 3 < 255; j += 4) {
        bb[j + 1][i]     = prefix[j]     + prev_bb;
        bb[j + 2][i]     = prefix[j + 1] + prev_bb;
        bb[j + 3][i]     = prefix[j + 2] + prev_bb;
        bb[j + 4][i]     = prefix[j + 3] + prev_bb;
    }
    for (; j < 255; j++) {
        bb[j + 1][i] = prefix[j] + prev_bb;
    }
}
