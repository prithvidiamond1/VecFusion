#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    int mid = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // First half: i from 0 to mid-1
        // Reads a[LEN_1D-i-1] where LEN_1D-i-1 >= i (unmodified a elements)
        for (int i = 0; i < mid; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }

        // Second half: i from mid to LEN_1D-1
        // Reads a[LEN_1D-i-1] where LEN_1D-i-1 < i (already modified a elements)
        for (int i = mid; i < LEN_1D; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }
}

        #include <stdint.h>

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    int mid = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // First half: i from 0 to mid-1
        // a[LEN_1D-i-1] is unmodified (indices from LEN_1D-1 down to mid)
        // These reads don't overlap with writes (writes go to a[0..mid-1])
        // so this loop can be vectorized.
        int i = 0;
        // Vector width: 4 floats
        for (; i <= mid - 4; i += 4) {
            int j0 = LEN_1D - i - 1;
            int j1 = LEN_1D - (i+1) - 1;
            int j2 = LEN_1D - (i+2) - 1;
            int j3 = LEN_1D - (i+3) - 1;

            float x0 = a[j0] + b[i+0] * c[i+0];
            float x1 = a[j1] + b[i+1] * c[i+1];
            float x2 = a[j2] + b[i+2] * c[i+2];
            float x3 = a[j3] + b[i+3] * c[i+3];

            a[i+0] = x0 - 1.0f;
            a[i+1] = x1 - 1.0f;
            a[i+2] = x2 - 1.0f;
            a[i+3] = x3 - 1.0f;

            b[i+0] = x0;
            b[i+1] = x1;
            b[i+2] = x2;
            b[i+3] = x3;
        }
        // Scalar cleanup for first half
        for (; i < mid; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }

        // Second half: i from mid to LEN_1D-1
        // a[LEN_1D-i-1] reads from already-modified a elements (indices 0..mid-1)
        // There are dependencies here, so we keep this scalar.
        for (int i = mid; i < LEN_1D; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
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
