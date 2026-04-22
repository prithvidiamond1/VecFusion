#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void vectorized_s322(int iterations, int LEN_1D, float* a, float* b, float* c) {
    for (int nl = 0; nl < iterations/2; nl++) {
        float* orig_a = (float*)malloc(LEN_1D * sizeof(float));
        if (orig_a == NULL) return;
        memcpy(orig_a, a, LEN_1D * sizeof(float));

        // Handle first two elements
        if (LEN_1D > 1) {
            a[1] = orig_a[1] + orig_a[0] * b[1];
        }

        // Vectorized main loop
        int i = 2;
        for (; i + 3 < LEN_1D; i += 4) {
            // Load original values for current iteration
            float orig_i0 = orig_a[i];
            float orig_i1 = orig_a[i+1];
            float orig_i2 = orig_a[i+2];
            float orig_i3 = orig_a[i+3];

            // Load dependency values from original array
            float orig_im2_0 = orig_a[i-2];
            float orig_im2_1 = orig_a[i-1];
            float orig_im2_2 = orig_a[i];
            float orig_im2_3 = orig_a[i+1];

            float orig_im1_0 = orig_a[i-1];
            float orig_im1_1 = orig_a[i];
            float orig_im1_2 = orig_a[i+1];
            float orig_im1_3 = orig_a[i+2];

            // Compute temp = orig_a[i] + orig_a[i-2] * c[i]
            float temp0 = orig_i0 + orig_im2_0 * c[i];
            float temp1 = orig_i1 + orig_im2_1 * c[i+1];
            float temp2 = orig_i2 + orig_im2_2 * c[i+2];
            float temp3 = orig_i3 + orig_im2_3 * c[i+3];

            // Compute a[i] = temp + orig_a[i-1] * b[i]
            a[i]   = temp0 + orig_im1_0 * b[i];
            a[i+1] = temp1 + orig_im1_1 * b[i+1];
            a[i+2] = temp2 + orig_im1_2 * b[i+2];
            a[i+3] = temp3 + orig_im1_3 * b[i+3];
        }

        // Scalar cleanup - also uses original values for dependencies
        for (; i < LEN_1D; i++) {
            float temp = orig_a[i] + orig_a[i-2] * c[i];
            a[i] = temp + orig_a[i-1] * b[i];
        }

        free(orig_a);
    }
}
