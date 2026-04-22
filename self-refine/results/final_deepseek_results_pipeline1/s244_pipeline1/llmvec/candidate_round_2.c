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
        // Handle first element separately to break the recurrence
        if (n >= 1) {
            float b0 = b[0];
            float c0 = c[0];
            float d0 = d[0];
            a[0] = b0 + c0 * d0;
            b[0] = c0 + b0;
            a[1] = b[0] + a[1] * d0;
        }

        // Vectorize remaining elements starting from i=1
        int i = 1;
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
            // Now safe because a[i+1] was not modified by previous vector iteration
            float_vec va_next = *(float_vec*)(a + i + 1);
            va_next = vb + va_next * vd;
            *(float_vec*)(a + i + 1) = va_next;
        }

        // Scalar cleanup
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
