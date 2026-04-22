#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
            b[i] = b[i + 1] - e[i] * d[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Note: b[i] depends on b[i+1] and b[i-1] is used for a[i],
        // but b[i-1] for a[i] uses the OLD b[i-1] before it was updated.
        // The loop updates b[i] = b[i+1] - e[i]*d[i], and a[i] = b[i-1] + c[i]*d[i]
        // where b[i-1] in a[i] refers to the ALREADY UPDATED b[i-1] from previous iteration step.
        // This is a loop-carried dependency on b[], so we must be careful.
        // 
        // Specifically:
        //   a[i] = b[i-1] + c[i]*d[i]   <- uses b[i-1] which was written at step i-1
        //   b[i] = b[i+1] - e[i]*d[i]   <- reads b[i+1] (not yet written), writes b[i]
        //
        // b[i] reads b[i+1] (forward reference, not yet modified), so b updates are safe to vectorize
        // a[i] reads b[i-1] which IS the updated b[i-1] from previous i step.
        // So a[i] depends on updated b[i-1]. We cannot fully vectorize both together.
        //
        // Strategy: compute a[] and b[] in two separate passes per outer iteration.
        // Pass 1: compute all a[i] using current b[] values (b[i-1] before any update)
        // Pass 2: compute all b[i] using current b[] values (b[i+1] not yet overwritten going forward)
        //
        // Wait - in the scalar loop, a[i] uses the UPDATED b[i-1] (from step i-1).
        // To preserve semantics exactly, we cannot split into two passes naively.
        // We must keep the scalar loop order for correctness.
        //
        // However, we can unroll the scalar loop to improve ILP.

        int i;
        int limit = LEN_1D - 1;
        // Unroll by 4, but since there's a loop-carried dep on b[], scalar unrolling only
        for (i = 1; i + 3 < limit; i += 4) {
            float b_im1_0 = b[i - 1];
            a[i] = b_im1_0 + c[i] * d[i];
            b[i] = b[i + 1] - e[i] * d[i];

            float b_im1_1 = b[i];  // updated b[i] used as b[(i+1)-1]
            a[i+1] = b_im1_1 + c[i+1] * d[i+1];
            b[i+1] = b[i + 2] - e[i+1] * d[i+1];

            float b_im1_2 = b[i+1];
            a[i+2] = b_im1_2 + c[i+2] * d[i+2];
            b[i+2] = b[i + 3] - e[i+2] * d[i+2];

            float b_im1_3 = b[i+2];
            a[i+3] = b_im1_3 + c[i+3] * d[i+3];
            b[i+3] = b[i + 4] - e[i+3] * d[i+3];
        }
        // scalar tail
        for (; i < limit; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
            b[i] = b[i + 1] - e[i] * d[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s211(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s211(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
