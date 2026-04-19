#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int half_len = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // Phase 1: Compute all unconditional results
        for (int i = 0; i < half_len; i++) {
            float de = d[i] * e[i];
            a[2*i] = b[i] + de;  // Store at even positions
        }

        // Phase 2: Compute conditional results
        for (int i = 0; i < half_len; i++) {
            float de = d[i] * e[i];
            float cond_val = c[i] + de;
            int mask = c[i] > 0.0f;
            // Store conditional result at odd positions
            a[2*i + 1] = cond_val * mask + a[2*i + 1] * (1 - mask);
        }

        // Phase 3: Compact results
        int dst = 0;
        for (int i = 0; i < half_len; i++) {
            // Always write unconditional element
            a[dst] = a[2*i];
            dst++;

            // Write conditional element if valid
            if (a[2*i + 1] != a[2*i]) {  // Check if conditional was computed
                a[dst] = a[2*i + 1];
                dst++;
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int half_len = LEN_1D / 2;

    for (int nl = 0; nl < iterations; nl++) {
        // Phase 1: Compute all unconditional results
        for (int i = 0; i < half_len; i++) {
            float de = d[i] * e[i];
            a[2*i] = b[i] + de;
        }

        // Phase 2: Compute conditional results
        for (int i = 0; i < half_len; i++) {
            float de = d[i] * e[i];
            float cond_val = c[i] + de;
            int mask = c[i] > 0.0f;
            a[2*i + 1] = cond_val * mask + a[2*i + 1] * (1 - mask);
        }

        // Phase 3: Compact results
        int dst = 0;
        for (int i = 0; i < half_len; i++) {
            a[dst] = a[2*i];
            dst++;

            if (a[2*i + 1] != a[2*i]) {
                a[dst] = a[2*i + 1];
                dst++;
            }
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
                s123(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s123(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
