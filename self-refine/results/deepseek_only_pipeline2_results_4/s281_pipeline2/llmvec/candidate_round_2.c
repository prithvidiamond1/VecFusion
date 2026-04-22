#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Process symmetric pairs together to break the dependency
        int i = 0;
        int j = LEN_1D - 1;

        // Process pairs from both ends towards the middle
        for (; i < j; i++, j--) {
            // Process element i (reads from original a[j])
            float x_i = a[j] + b[i] * c[i];
            a[i] = x_i - 1.0f;
            b[i] = x_i;

            // Process element j (reads from original a[i] which hasn't been updated yet)
            float x_j = a[i] + b[j] * c[j];
            a[j] = x_j - 1.0f;
            b[j] = x_j;
        }

        // Handle middle element when LEN_1D is odd
        if (i == j) {
            float x = a[i] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }
    }
}

        #include <stdint.h>

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c) {
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        int j = LEN_1D - 1;

        // Process 4 pairs at a time using vectorization
        for (; i + 3 < j - 3; i += 4, j -= 4) {
            // Load original a[i..i+3] values BEFORE they are updated
            float a_i0_orig = a[i];
            float a_i1_orig = a[i+1];
            float a_i2_orig = a[i+2];
            float a_i3_orig = a[i+3];

            // Load b[i..i+3] and c[i..i+3]
            float b_i0 = b[i];
            float b_i1 = b[i+1];
            float b_i2 = b[i+2];
            float b_i3 = b[i+3];

            float c_i0 = c[i];
            float c_i1 = c[i+1];
            float c_i2 = c[i+2];
            float c_i3 = c[i+3];

            // Load b[j-3..j] and c[j-3..j]
            float b_j3 = b[j-3];
            float b_j2 = b[j-2];
            float b_j1 = b[j-1];
            float b_j0 = b[j];

            float c_j3 = c[j-3];
            float c_j2 = c[j-2];
            float c_j1 = c[j-1];
            float c_j0 = c[j];

            // Load a[j..j-3] for i elements (reverse order)
            float a_j0 = a[j];
            float a_j1 = a[j-1];
            float a_j2 = a[j-2];
            float a_j3 = a[j-3];

            // Compute x_i = a[j] + b[i] * c[i]
            float x_i0 = a_j0 + b_i0 * c_i0;
            float x_i1 = a_j1 + b_i1 * c_i1;
            float x_i2 = a_j2 + b_i2 * c_i2;
            float x_i3 = a_j3 + b_i3 * c_i3;

            // Store a[i] = x_i - 1.0f and b[i] = x_i
            a[i] = x_i0 - 1.0f;
            a[i+1] = x_i1 - 1.0f;
            a[i+2] = x_i2 - 1.0f;
            a[i+3] = x_i3 - 1.0f;

            b[i] = x_i0;
            b[i+1] = x_i1;
            b[i+2] = x_i2;
            b[i+3] = x_i3;

            // Compute x_j = a[i]_orig + b[j] * c[j] using original a[i] values
            float x_j3 = a_i0_orig + b_j3 * c_j3;  // j-3
            float x_j2 = a_i1_orig + b_j2 * c_j2;  // j-2
            float x_j1 = a_i2_orig + b_j1 * c_j1;  // j-1
            float x_j0 = a_i3_orig + b_j0 * c_j0;  // j

            // Store a[j] = x_j - 1.0f and b[j] = x_j
            a[j-3] = x_j3 - 1.0f;
            a[j-2] = x_j2 - 1.0f;
            a[j-1] = x_j1 - 1.0f;
            a[j] = x_j0 - 1.0f;

            b[j-3] = x_j3;
            b[j-2] = x_j2;
            b[j-1] = x_j1;
            b[j] = x_j0;
        }

        // Process remaining pairs
        for (; i < j; i++, j--) {
            float x_i = a[j] + b[i] * c[i];
            a[i] = x_i - 1.0f;
            b[i] = x_i;

            float x_j = a[i] + b[j] * c[j];
            a[j] = x_j - 1.0f;
            b[j] = x_j;
        }

        // Handle middle element when LEN_1D is odd
        if (i == j) {
            float x = a[i] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }
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
                s281(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s281(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
