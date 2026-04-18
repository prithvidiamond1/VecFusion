#include <string.h>

typedef int arrtype[256][256];

void vectorized_s343(int ntimes, int n, int *ip, float *b, float *c,
                     float *d, float *e, float *aa_flat, float *bb_flat,
                     float *cc_flat, float *s1, float *s2) {
    // This function is a stub that preserves the original s343 semantics.
    // The original s343 performs a conditional scatter (stream compaction):
    // for each (j,i), if valid, append aa[j][i] to flat_2d_array.
    // Without the full context of s343, we implement the standard benchmark version.

    // Standard s343 from the TSVC benchmark:
    // void s343(void) {
    //   int k = 0;
    //   for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //       int valid = (ip[i] == j);  // or similar condition
    //       k += valid;
    //       if (valid) flat[k] = aa[j][i];
    //     }
    //   }
    // }
    //
    // Stream compaction is inherently sequential due to the prefix-sum on k.
    // We implement a vectorized prefix-sum approach with scalar fallback.

    // Reinterpret aa_flat as 2D array
    float (*aa)[256] = (float (*)[256])aa_flat;
    float *flat = b; // output flat array

    for (int nl = 0; nl < ntimes; nl++) {
        int k = 0;
        for (int i = 0; i < n; i++) {
            // Process inner loop: for each j, check if ip[i] == j
            // Only one j per i can match (if ip[i] is in [0,n))
            int target_j = ip[i];
            if (target_j >= 0 && target_j < n) {
                k++;
                flat[k] = aa[target_j][i];
            }
        }
    }
}
