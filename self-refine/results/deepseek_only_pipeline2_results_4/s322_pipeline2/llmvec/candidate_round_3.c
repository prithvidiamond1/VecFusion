#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s322(int iterations, int LEN_1D, float* a, float* b, float* c) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original values to break loop-carried dependency
        float* orig_a = (float*)malloc(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; i++) {
            orig_a[i] = a[i];
        }

        // Process first two elements separately
        if (LEN_1D > 1) {
            a[1] = orig_a[1] + orig_a[0] * b[1];
        }

        // Main loop - split into two independent computations
        for (int i = 2; i < LEN_1D; i++) {
            float temp = orig_a[i] + a[i-2] * c[i];
            a[i] = temp + a[i-1] * b[i];
        }

        free(orig_a);
    }
}

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

            // Load dependency values from UPDATED a array
            float a_im2_0 = a[i-2];
            float a_im2_1 = a[i-1];
            float a_im2_2 = a[i];
            float a_im2_3 = a[i+1];

            float a_im1_0 = a[i-1];
            float a_im1_1 = a[i];
            float a_im1_2 = a[i+1];
            float a_im1_3 = a[i+2];

            // Compute temp = orig_a[i] + a[i-2] * c[i]
            float temp0 = orig_i0 + a_im2_0 * c[i];
            float temp1 = orig_i1 + a_im2_1 * c[i+1];
            float temp2 = orig_i2 + a_im2_2 * c[i+2];
            float temp3 = orig_i3 + a_im2_3 * c[i+3];

            // Compute a[i] = temp + a[i-1] * b[i]
            a[i]   = temp0 + a_im1_0 * b[i];
            a[i+1] = temp1 + a_im1_1 * b[i+1];
            a[i+2] = temp2 + a_im1_2 * b[i+2];
            a[i+3] = temp3 + a_im1_3 * b[i+3];
        }

        // Scalar cleanup - uses updated a values for dependencies
        for (; i < LEN_1D; i++) {
            float temp = orig_a[i] + a[i-2] * c[i];
            a[i] = temp + a[i-1] * b[i];
        }

        free(orig_a);
    }
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s322(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s322(iterations, LEN_1D, a_vector, b_vector, c_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
