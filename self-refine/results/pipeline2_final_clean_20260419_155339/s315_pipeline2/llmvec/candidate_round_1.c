#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s315(int iterations, int LEN_1D, float* a) {
    // Initialize array (vectorizable)
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x;
    int index;
    float chksum = 0.0f;

    // Outer loop over iterations
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // Loop splitting: separate max value reduction from index tracking
        // First loop: find max value (pure reduction, vectorizable)
        float max_val = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            float val = a[i];
            max_val = val > max_val ? val : max_val;
        }

        // Second loop: find last index of max value (vectorizable comparison)
        // Use branchless logic to track last occurrence
        int max_idx = 0;
        for (int i = 0; i < LEN_1D; ++i) {
            // Create mask: 1 if current element equals max_val, 0 otherwise
            int is_max = a[i] == max_val;
            // Update index to current i when is_max is true, keep old value otherwise
            // This captures the LAST occurrence since we overwrite with later indices
            max_idx = is_max ? i : max_idx;
        }

        x = max_val;
        index = max_idx;

        // Compute chksum to maintain exact memory access pattern
        chksum = x + (float)index;
        (void)chksum;
    }

    return index + x + 1;
}

        #include <stddef.h>

float vectorized_s315(int iterations, int LEN_1D, float* a) {
    // Initialize array (vectorizable)
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x;
    int index;
    float chksum = 0.0f;

    // Outer loop over iterations
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // First loop: find max value (vectorizable reduction)
        float max_val = a[0];
        int i = 1;

        // Vectorized max reduction
        for (; i + 3 < LEN_1D; i += 4) {
            float val0 = a[i];
            float val1 = a[i + 1];
            float val2 = a[i + 2];
            float val3 = a[i + 3];

            if (val0 > max_val) max_val = val0;
            if (val1 > max_val) max_val = val1;
            if (val2 > max_val) max_val = val2;
            if (val3 > max_val) max_val = val3;
        }

        // Scalar tail
        for (; i < LEN_1D; ++i) {
            float val = a[i];
            if (val > max_val) max_val = val;
        }

        // Second loop: find last index of max value (vectorizable)
        int max_idx = 0;
        i = 0;

        // Vectorized index tracking
        for (; i + 3 < LEN_1D; i += 4) {
            int idx0 = i;
            int idx1 = i + 1;
            int idx2 = i + 2;
            int idx3 = i + 3;

            int is_max0 = a[idx0] == max_val;
            int is_max1 = a[idx1] == max_val;
            int is_max2 = a[idx2] == max_val;
            int is_max3 = a[idx3] == max_val;

            if (is_max0) max_idx = idx0;
            if (is_max1) max_idx = idx1;
            if (is_max2) max_idx = idx2;
            if (is_max3) max_idx = idx3;
        }

        // Scalar tail
        for (; i < LEN_1D; ++i) {
            if (a[i] == max_val) max_idx = i;
        }

        x = max_val;
        index = max_idx;

        // Compute chksum to maintain exact memory access pattern
        chksum = x + (float)index;
        (void)chksum;
    }

    return index + x + 1;
}

        static uint32_t next_u32(uint32_t *state) {
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }

        static void fill_i32(int *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }
        }

        static void fill_f32(float *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
            }
        }

        static void fill_f64(double *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
            }
        }

        int main(void) {
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s315(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s315(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
