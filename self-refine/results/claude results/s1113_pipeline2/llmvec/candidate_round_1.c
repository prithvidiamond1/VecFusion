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
        float mid_val = a[mid];
        for (int i = 0; i < mid; i++) {
            a[i] = mid_val + b[i];
        }
        a[mid] = mid_val + b[mid];
        float new_mid_val = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = new_mid_val + b[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float mid_val = a[mid];

        // Vectorize first loop: i = 0 to mid-1
        int i = 0;
        int mid_vec_end = mid - (mid % 8);
        for (; i < mid_vec_end; i += 8) {
            a[i+0] = mid_val + b[i+0];
            a[i+1] = mid_val + b[i+1];
            a[i+2] = mid_val + b[i+2];
            a[i+3] = mid_val + b[i+3];
            a[i+4] = mid_val + b[i+4];
            a[i+5] = mid_val + b[i+5];
            a[i+6] = mid_val + b[i+6];
            a[i+7] = mid_val + b[i+7];
        }
        for (; i < mid; i++) {
            a[i] = mid_val + b[i];
        }

        // Update mid element
        a[mid] = mid_val + b[mid];
        float new_mid_val = a[mid];

        // Vectorize second loop: i = mid+1 to LEN_1D-1
        int len2 = LEN_1D - (mid + 1);
        int start2 = mid + 1;
        int vec_end2 = start2 + (len2 - (len2 % 8));
        i = start2;
        for (; i < vec_end2; i += 8) {
            a[i+0] = new_mid_val + b[i+0];
            a[i+1] = new_mid_val + b[i+1];
            a[i+2] = new_mid_val + b[i+2];
            a[i+3] = new_mid_val + b[i+3];
            a[i+4] = new_mid_val + b[i+4];
            a[i+5] = new_mid_val + b[i+5];
            a[i+6] = new_mid_val + b[i+6];
            a[i+7] = new_mid_val + b[i+7];
        }
        for (; i < LEN_1D; i++) {
            a[i] = new_mid_val + b[i];
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s1113(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s1113(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
