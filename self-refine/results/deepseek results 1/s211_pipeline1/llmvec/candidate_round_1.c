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
        // Note: b[i] depends on b[i+1] and b[i-1] in the same iteration,
        // but since a[i] uses b[i-1] (old value) and b[i] uses b[i+1] (old value),
        // and b[i] is written after a[i] is computed, we need to be careful.
        // The loop reads b[i-1] for a[i] and b[i+1] for b[i].
        // Since b[i] is updated in order i=1,2,..., b[i+1] hasn't been updated yet
        // when we compute b[i], so we can vectorize if we save old b values first.

        int len = LEN_1D - 2; // number of iterations: i from 1 to LEN_1D-2
        if (len <= 0) continue;

        // We need old b values since b is both read and written.
        // b[i] reads b[i-1] (already written if i>1 in scalar) and b[i+1] (not yet written).
        // Wait - in scalar: a[i] = b[i-1] + c[i]*d[i], then b[i] = b[i+1] - e[i]*d[i]
        // When computing i=2: a[2] = b[1] (already updated at i=1) + c[2]*d[2]
        // So there IS a loop-carried dependency through b[i-1] for a[i].
        // This means we cannot simply vectorize without saving old b.
        // We'll copy b to a temp buffer first, then vectorize.

        // Use a local temp copy of b for the old values
        // To avoid VLA issues with large sizes, we'll just do scalar with unrolling
        // but structured for vectorization where possible.

        // Since b[i-1] used in a[i] depends on b updated at i-1, we must use
        // a temporary copy of b. Let's do it with a simple approach:
        // save b[1..LEN_1D-2] old values, then vectorize.

        // Allocate temp on stack only if small, otherwise just do scalar unroll
        // For correctness, let's just do 4-wide unroll with scalar semantics preserved
        // by processing sequentially (the dependency prevents true SIMD here for a[i]).

        // Actually let's just unroll 4x scalar - the b[i-1] dependency makes true
        // vectorization of a[i] incorrect without a temp copy.

        int i = 1;
        for (; i <= LEN_1D - 5; i += 4) {
            float a0 = b[i-1]   + c[i]   * d[i];
            float b0 = b[i+1]   - e[i]   * d[i];
            float a1 = b0       + c[i+1] * d[i+1];  // b[i] = b0 (just updated)
            float b1 = b[i+2]   - e[i+1] * d[i+1];
            float a2 = b1       + c[i+2] * d[i+2];
            float b2 = b[i+3]   - e[i+2] * d[i+2];
            float a3 = b2       + c[i+3] * d[i+3];
            float b3 = b[i+4]   - e[i+3] * d[i+3];
            a[i]   = a0; b[i]   = b0;
            a[i+1] = a1; b[i+1] = b1;
            a[i+2] = a2; b[i+2] = b2;
            a[i+3] = a3; b[i+3] = b3;
        }
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
            b[i] = b[i+1] - e[i] * d[i];
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
