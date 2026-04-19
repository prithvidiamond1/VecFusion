#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1113(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;

    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Split the loop into three parts to handle the dependency on a[mid]
        // Part 1: i < mid (uses original a[mid] value)
        float mid_val = a[mid];
        for (int i = 0; i < mid; i++) {
            a[i] = mid_val + b[i];
        }

        // Part 2: i == mid (update a[mid] using its original value)
        a[mid] = mid_val + b[mid];

        // Part 3: i > mid (uses the updated a[mid] value)
        float updated_mid_val = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = updated_mid_val + b[i];
        }
    }
}

        #include <stddef.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;

    for (int nl = 0; nl < 2 * iterations; nl++) {
        float mid_val = a[mid];

        // Vectorize first part: i < mid
        int i = 0;
        for (; i + 3 < mid; i += 4) {
            a[i] = mid_val + b[i];
            a[i + 1] = mid_val + b[i + 1];
            a[i + 2] = mid_val + b[i + 2];
            a[i + 3] = mid_val + b[i + 3];
        }
        // Scalar tail for first part
        for (; i < mid; i++) {
            a[i] = mid_val + b[i];
        }

        // Middle element
        a[mid] = mid_val + b[mid];

        // Vectorize second part: i > mid
        float updated_mid_val = a[mid];
        i = mid + 1;
        for (; i + 3 < LEN_1D; i += 4) {
            a[i] = updated_mid_val + b[i];
            a[i + 1] = updated_mid_val + b[i + 1];
            a[i + 2] = updated_mid_val + b[i + 2];
            a[i + 3] = updated_mid_val + b[i + 3];
        }
        // Scalar tail for second part
        for (; i < LEN_1D; i++) {
            a[i] = updated_mid_val + b[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s1113(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s1113(iterations, LEN_1D, a_vector, b_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
