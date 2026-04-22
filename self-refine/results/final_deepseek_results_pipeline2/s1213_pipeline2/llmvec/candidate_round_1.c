#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original a[i+1] values before they are overwritten
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = a[i+1] * d[i];
        }
        // Now compute a[i] using original b[i-1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i];
        }
    }
}

        typedef float float4 __attribute__((vector_size(16)));

void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    // Compute loop bounds for vectorized portion
    int start = 1;
    int end = LEN_1D - 1;
    int vec_len = end - start;

    // Process all iterations
    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized store of a[i+1] * d[i]
        int i = start;
        for (; i + 3 < end; i += 4) {
            float4 a_vec = {a[i+1], a[i+2], a[i+3], a[i+4]};
            float4 d_vec = {d[i], d[i+1], d[i+2], d[i+3]};
            float4 b_vec = a_vec * d_vec;
            b[i] = b_vec[0];
            b[i+1] = b_vec[1];
            b[i+2] = b_vec[2];
            b[i+3] = b_vec[3];
        }
        // Scalar tail for first loop
        for (; i < end; i++) {
            b[i] = a[i+1] * d[i];
        }

        // Vectorized computation of b[i-1] + c[i]
        i = start;
        for (; i + 3 < end; i += 4) {
            float4 b_prev = {b[i-1], b[i], b[i+1], b[i+2]};
            float4 c_vec = {c[i], c[i+1], c[i+2], c[i+3]};
            float4 a_vec = b_prev + c_vec;
            a[i] = a_vec[0];
            a[i+1] = a_vec[1];
            a[i+2] = a_vec[2];
            a[i+3] = a_vec[3];
        }
        // Scalar tail for second loop
        for (; i < end; i++) {
            a[i] = b[i-1] + c[i];
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
                s1213(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s1213(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
