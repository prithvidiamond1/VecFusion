#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d) {
    float s_sum = s1 + s2;

    for (int nl = 0; nl < iterations/5; nl++) {
        float prev = a[0];

        // Compute independent parts and store in a temporary array
        float* temp = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        for (int i = 1; i < LEN_1D; ++i) {
            temp[i] = s_sum + b[i] + c[i] + d[i];
        }

        // Perform recurrence accumulation
        for (int i = 1; i < LEN_1D; ++i) {
            prev = prev + temp[i];
            a[i] = prev;
        }
    }
}

        #include <stddef.h>

void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d) {
    float s_sum = s1 + s2;
    int loop_bound = iterations / 5;

    for (int nl = 0; nl < loop_bound; nl++) {
        float prev = a[0];

        // Compute independent parts with explicit bounds
        int i = 1;
        int main_loop_bound = LEN_1D - 1;

        // Vector-friendly loop for independent computations
        for (; i <= main_loop_bound; i++) {
            // Independent computation - no dependencies
            a[i] = s_sum + b[i] + c[i] + d[i];
        }

        // Perform recurrence accumulation with explicit bounds
        prev = a[0];
        for (i = 1; i < LEN_1D; i++) {
            prev = prev + a[i];
            a[i] = prev;
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
            int iterations = 5; int LEN_1D = arr_len; float s1 = 7; float s2 = 7; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s242(iterations, LEN_1D, s1, s2, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s242(iterations, LEN_1D, s1, s2, a_vector, b_vector, c_vector, d_vector);
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
