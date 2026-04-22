#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    for (int nl = 0; nl < iterations; nl++) {
        // Loop distribution: separate the independent computations
        // First, compute all b[i] updates
        for (int i = 0; i < LEN_1D - 1; ++i) {
            b[i] = c[i] + b[i];
        }

        // Then compute a[i] and a[i+1] using the updated b[i]
        // Handle first iteration separately to break dependency
        int i = 0;
        float a_next_temp = a[i+1];  // Save original a[1]
        a[i] = b[i] + c[i] * d[i];
        a[i+1] = b[i] + a_next_temp * d[i];

        // Main vectorizable loop for remaining iterations
        for (i = 1; i < LEN_1D - 1; ++i) {
            a_next_temp = a[i+1];  // Save original a[i+1]
            a[i] = b[i] + c[i] * d[i];
            a[i+1] = b[i] + a_next_temp * d[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    typedef float v4f __attribute__((vector_size(16)));
    const int VF = 4;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorize b[i] = c[i] + b[i]
        int i = 0;
        for (; i <= LEN_1D - 1 - VF; i += VF) {
            v4f b_vec = *(v4f*)&b[i];
            v4f c_vec = *(v4f*)&c[i];
            v4f result = b_vec + c_vec;
            *(v4f*)&b[i] = result;
        }
        // Scalar tail for b updates
        for (; i < LEN_1D - 1; ++i) {
            b[i] = c[i] + b[i];
        }

        // Handle first iteration separately
        i = 0;
        float a_next_temp = a[i+1];
        a[i] = b[i] + c[i] * d[i];
        a[i+1] = b[i] + a_next_temp * d[i];

        // Main loop remains scalar due to cross-iteration dependencies
        for (i = 1; i < LEN_1D - 1; ++i) {
            float a_next_temp = a[i+1];
            a[i] = b[i] + c[i] * d[i];
            a[i+1] = b[i] + a_next_temp * d[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s244(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s244(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
