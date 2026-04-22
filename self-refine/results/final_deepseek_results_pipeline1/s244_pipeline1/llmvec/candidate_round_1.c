#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s244(int iterations, int LEN_1D, float* a,float* b,float* c,float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < LEN_1D-1; ++i) {
            a[i] = b[i] + c[i] * d[i];
            b[i] = c[i] + b[i];
            a[i+1] = b[i] + a[i+1] * d[i];
        }
    }
}

        #include <stddef.h>

typedef float float_vec __attribute__((ext_vector_type(4)));

void vectorized_s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    int n = LEN_1D - 1;
    if (n <= 0) return;

    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        // Vectorize with 4-wide SIMD, but note the loop-carried dependency on a[i+1]
        // We can vectorize by processing 4 elements at a time, handling the dependency carefully
        for (; i <= n - 4; i += 4) {
            float_vec vb = *(float_vec*)(b + i);
            float_vec vc = *(float_vec*)(c + i);
            float_vec vd = *(float_vec*)(d + i);

            // a[i] = b[i] + c[i] * d[i]
            float_vec va = vb + vc * vd;
            *(float_vec*)(a + i) = va;

            // b[i] = c[i] + b[i]
            vb = vc + vb;
            *(float_vec*)(b + i) = vb;

            // a[i+1] = b[i] + a[i+1] * d[i]
            // This has a dependency: a[i+1] is read then written
            // We need to handle the shift carefully
            float_vec va_next = *(float_vec*)(a + i + 1);
            float_vec vd_shift = vd; // d[i] for each element
            // For each lane j: a[i+1+j] = b[i+j] + a[i+1+j] * d[i+j]
            // But note: a[i+1] uses b[i], a[i+2] uses b[i+1], etc.
            // This is actually independent per lane when vectorized
            va_next = vb + va_next * vd_shift;
            *(float_vec*)(a + i + 1) = va_next;
        }

        // Scalar cleanup for remaining elements
        for (; i < n; i++) {
            a[i] = b[i] + c[i] * d[i];
            b[i] = c[i] + b[i];
            a[i+1] = b[i] + a[i+1] * d[i];
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
