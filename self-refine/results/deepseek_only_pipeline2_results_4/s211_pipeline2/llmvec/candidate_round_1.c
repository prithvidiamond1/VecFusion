#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Reorder instructions to break dependency chain
        // First compute all b[i] updates using original b[i+1]
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b[i + 1] - e[i] * d[i];
        }

        // Then compute all a[i] using the newly updated b[i-1]
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s211(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    typedef float v4f __attribute__((vector_size(16)));
    const int VEC_SIZE = 4;

    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;

        // Vectorized b[i] = b[i + 1] - e[i] * d[i];
        for (; i + VEC_SIZE <= LEN_1D - 1; i += VEC_SIZE) {
            v4f b_next = *(v4f*)&b[i + 1];
            v4f e_vec = *(v4f*)&e[i];
            v4f d_vec = *(v4f*)&d[i];
            v4f result = b_next - e_vec * d_vec;
            *(v4f*)&b[i] = result;
        }

        // Scalar tail for b updates
        for (; i < LEN_1D - 1; i++) {
            b[i] = b[i + 1] - e[i] * d[i];
        }

        i = 1;

        // Vectorized a[i] = b[i - 1] + c[i] * d[i];
        for (; i + VEC_SIZE <= LEN_1D - 1; i += VEC_SIZE) {
            v4f b_prev = *(v4f*)&b[i - 1];
            v4f c_vec = *(v4f*)&c[i];
            v4f d_vec = *(v4f*)&d[i];
            v4f result = b_prev + c_vec * d_vec;
            *(v4f*)&a[i] = result;
        }

        // Scalar tail for a updates
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
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
