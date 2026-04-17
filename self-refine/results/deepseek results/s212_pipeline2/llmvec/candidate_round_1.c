#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s212(int n, float *a, float *b, float *c, float *d) {
    // Store original a[i+1] values before modifying a[i]
    for (int i = 0; i < n - 1; i++) {
        float a_next_original = a[i + 1];
        a[i] *= c[i];
        b[i] += a_next_original * d[i];
    }
}

        #include <stdint.h>

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    // We need to read a[i+1] before modifying a[i].
    // Since a[i+1] is read as original before a[i] is written,
    // and a[i] = a[i] * c[i], a[i+1] original is just a[i+1] before any modification.
    // We can preload a[1..n-1] into a temporary buffer, then do vectorized ops.

    int limit = n - 1;
    if (limit <= 0) return;

    // Allocate a temporary array to hold original a[i+1] values
    // Use a local VLA or static buffer approach
    // To keep it simple and vectorizable, use a shifted pointer approach:
    // a_next[i] = a[i+1] for i in [0, limit)
    // We read all a[i+1] first (they are original since we haven't modified anything yet
    // for i=0: a[1] original, then a[0] *= c[0]
    // for i=1: a[2] original, then a[1] *= c[1]
    // So a[i+1] is always original at the time we read it in scalar loop.
    // But if we vectorize, a[i] *= c[i] for all i first, then a[i+1] would be modified.
    // So we must read a[i+1] = a[i+1] before any writes.

    // Strategy: read a[1..n-1] into temp, then vectorize.
    float temp[limit];

    // Load original a[i+1] values
    int i = 0;

    // Vectorized load using 4-wide unroll
    for (i = 0; i <= limit - 4; i += 4) {
        temp[i]     = a[i + 1];
        temp[i + 1] = a[i + 2];
        temp[i + 2] = a[i + 3];
        temp[i + 3] = a[i + 4];
    }
    for (; i < limit; i++) {
        temp[i] = a[i + 1];
    }

    // Now vectorize the main computation
    // a[i] *= c[i]
    // b[i] += temp[i] * d[i]
    i = 0;
    for (i = 0; i <= limit - 8; i += 8) {
        a[i]     *= c[i];
        a[i + 1] *= c[i + 1];
        a[i + 2] *= c[i + 2];
        a[i + 3] *= c[i + 3];
        a[i + 4] *= c[i + 4];
        a[i + 5] *= c[i + 5];
        a[i + 6] *= c[i + 6];
        a[i + 7] *= c[i + 7];

        b[i]     += temp[i]     * d[i];
        b[i + 1] += temp[i + 1] * d[i + 1];
        b[i + 2] += temp[i + 2] * d[i + 2];
        b[i + 3] += temp[i + 3] * d[i + 3];
        b[i + 4] += temp[i + 4] * d[i + 4];
        b[i + 5] += temp[i + 5] * d[i + 5];
        b[i + 6] += temp[i + 6] * d[i + 6];
        b[i + 7] += temp[i + 7] * d[i + 7];
    }
    for (; i < limit; i++) {
        a[i] *= c[i];
        b[i] += temp[i] * d[i];
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
            int n = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s212(n, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s212(n, a_vector, b_vector, c_vector, d_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
