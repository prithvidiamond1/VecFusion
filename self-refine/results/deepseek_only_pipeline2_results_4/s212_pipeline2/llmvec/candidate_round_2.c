#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s212(int n, float *a, float *b, float *c, float *d) {
    // Store original a[i+1] values before they are modified
    float *a_next = (float *)malloc((n-1) * sizeof(float));
    if (a_next == NULL) return;

    // First loop: compute a[i] *= c[i] and save original a[i+1]
    for (int i = 0; i < n-1; i++) {
        a_next[i] = a[i+1];  // Save original value before modification
        a[i] *= c[i];
    }

    // Second loop: compute b[i] += saved_a_next * d[i]
    for (int i = 0; i < n-1; i++) {
        b[i] += a_next[i] * d[i];
    }

    free(a_next);
}

        #include <stddef.h>
#include <stdlib.h>

typedef float v8f __attribute__((__vector_size__(32), __aligned__(32)));

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    if (n <= 1) return;

    float *a_next = (float *)malloc((n-1) * sizeof(float));
    if (a_next == NULL) return;

    const int vlen = 8;
    int loop_limit = n - 1;
    int i = 0;
    int limit = loop_limit - (loop_limit % vlen);

    // First loop: save a[i+1] and compute a[i] *= c[i]
    for (; i < limit; i += vlen) {
        // Load a[i+1] for saving (original values before any modification)
        v8f a_next_vec = *(v8f *)&a[i+1];
        *(v8f *)&a_next[i] = a_next_vec;

        // Load a[i] and c[i], compute a[i] *= c[i]
        v8f a_vec = *(v8f *)&a[i];
        v8f c_vec = *(v8f *)&c[i];
        a_vec = a_vec * c_vec;
        *(v8f *)&a[i] = a_vec;
    }

    // Scalar tail for first loop
    for (; i < loop_limit; i++) {
        a_next[i] = a[i+1];
        a[i] *= c[i];
    }

    // Second loop: b[i] += saved_a_next * d[i]
    i = 0;
    limit = loop_limit - (loop_limit % vlen);
    for (; i < limit; i += vlen) {
        v8f b_vec = *(v8f *)&b[i];
        v8f a_next_vec = *(v8f *)&a_next[i];
        v8f d_vec = *(v8f *)&d[i];
        b_vec = b_vec + a_next_vec * d_vec;
        *(v8f *)&b[i] = b_vec;
    }

    // Scalar tail for second loop
    for (; i < loop_limit; i++) {
        b[i] += a_next[i] * d[i];
    }

    free(a_next);
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
