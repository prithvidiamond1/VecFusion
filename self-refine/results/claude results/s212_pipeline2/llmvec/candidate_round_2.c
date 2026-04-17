#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s212(int n, float *a, float *b, float *c, float *d) {
    // Save original a[1..n-1] values before they get updated
    // b[i] uses a[i+1] which is the ORIGINAL (not yet multiplied by c[i+1]) value
    // We need to store original a values first, then do the updates

    // Use a temp copy of a[1..n-1] (the values used by b[i])
    float *a_orig = (float *)__builtin_alloca((n - 1) * sizeof(float));
    memcpy(a_orig, a + 1, (n - 1) * sizeof(float));

    // Now update a[i] *= c[i] - vectorizable
    for (int i = 0; i < n - 1; i++) {
        a[i] *= c[i];
    }

    // Now update b[i] using original a[i+1] values - vectorizable
    for (int i = 0; i < n - 1; i++) {
        b[i] += a_orig[i] * d[i];
    }
}

        #include <string.h>

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    float *a_orig = (float *)__builtin_alloca((n - 1) * sizeof(float));
    memcpy(a_orig, a + 1, (n - 1) * sizeof(float));

    int i = 0;
    int len = n - 1;

#if defined(__clang__) || defined(__GNUC__)
    typedef float float4 __attribute__((vector_size(16)));

    int vec_len = len - (len % 4);

    for (i = 0; i < vec_len; i += 4) {
        float4 av, cv, bv, a_orig_v, dv;

        __builtin_memcpy(&av, &a[i], 16);
        __builtin_memcpy(&cv, &c[i], 16);
        av = av * cv;
        __builtin_memcpy(&a[i], &av, 16);

        __builtin_memcpy(&bv, &b[i], 16);
        __builtin_memcpy(&a_orig_v, &a_orig[i], 16);
        __builtin_memcpy(&dv, &d[i], 16);
        bv = bv + a_orig_v * dv;
        __builtin_memcpy(&b[i], &bv, 16);
    }

    for (; i < len; i++) {
        a[i] *= c[i];
        b[i] += a_orig[i] * d[i];
    }
#else
    for (i = 0; i < len; i++) {
        a[i] *= c[i];
    }
    for (i = 0; i < len; i++) {
        b[i] += a_orig[i] * d[i];
    }
#endif
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
            int n = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s212(n, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s212(n, a_vector, b_vector, c_vector, d_vector);
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
