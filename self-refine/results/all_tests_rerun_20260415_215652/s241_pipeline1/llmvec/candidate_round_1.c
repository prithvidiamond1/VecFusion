#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s241(int iterations, int LEN_1D, float* a,float* b,float* c,float* d)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        for (int i = 0; i < LEN_1D-1; i++) {
            a[i] = b[i] * c[i  ] * d[i];
            b[i] = a[i] * a[i+1] * d[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // The inner loop has a loop-carried dependency through a[i+1] in the b[i] update,
        // but since a[i+1] is written in the same iteration (when i becomes i+1),
        // we need to be careful. However, b[i] uses a[i+1] which is the NEW a[i+1]
        // (written when the loop index is i+1). So b[i] depends on a[i+1] which
        // hasn't been computed yet in iteration i. This means b[i] uses the OLD a[i+1].
        // Wait - let's re-read: a[i] = b[i]*c[i]*d[i], then b[i] = a[i]*a[i+1]*d[i].
        // a[i+1] here is the value from the PREVIOUS iteration of nl (or initial),
        // because in the current i-loop iteration, a[i+1] hasn't been updated yet
        // (it will be updated when i increments). So a[i+1] is the old value.
        // This means the a[] update and b[] update within the same i are independent
        // of each other across different i values (no loop-carried dependency in a[]).
        // b[i] uses a[i] (just computed) and a[i+1] (not yet computed in this nl pass).
        // So we can vectorize by first computing all a[], then all b[].

        int i = 0;
        int limit = LEN_1D - 1;

        // Vectorize using width 8 unrolling
        int limit8 = limit - (limit % 8);

        // First pass: compute all a[i] for i in [0, LEN_1D-2]
        for (i = 0; i < limit8; i += 8) {
            a[i+0] = b[i+0] * c[i+0] * d[i+0];
            a[i+1] = b[i+1] * c[i+1] * d[i+1];
            a[i+2] = b[i+2] * c[i+2] * d[i+2];
            a[i+3] = b[i+3] * c[i+3] * d[i+3];
            a[i+4] = b[i+4] * c[i+4] * d[i+4];
            a[i+5] = b[i+5] * c[i+5] * d[i+5];
            a[i+6] = b[i+6] * c[i+6] * d[i+6];
            a[i+7] = b[i+7] * c[i+7] * d[i+7];
        }
        for (; i < limit; i++) {
            a[i] = b[i] * c[i] * d[i];
        }

        // Second pass: compute all b[i] for i in [0, LEN_1D-2]
        // b[i] = a[i] * a[i+1] * d[i], using updated a[] values
        i = 0;
        for (i = 0; i < limit8; i += 8) {
            b[i+0] = a[i+0] * a[i+1] * d[i+0];
            b[i+1] = a[i+1] * a[i+2] * d[i+1];
            b[i+2] = a[i+2] * a[i+3] * d[i+2];
            b[i+3] = a[i+3] * a[i+4] * d[i+3];
            b[i+4] = a[i+4] * a[i+5] * d[i+4];
            b[i+5] = a[i+5] * a[i+6] * d[i+5];
            b[i+6] = a[i+6] * a[i+7] * d[i+6];
            b[i+7] = a[i+7] * a[i+8] * d[i+7];
        }
        for (; i < limit; i++) {
            b[i] = a[i] * a[i+1] * d[i];
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s241(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s241(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
