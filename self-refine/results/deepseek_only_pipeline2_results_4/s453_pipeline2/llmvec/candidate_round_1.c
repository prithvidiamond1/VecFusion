#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s453(int iterations, int LEN_1D, float* a, float *b)
{
    float s;
    for (int nl = 0; nl < iterations*2; nl++) {
        s = 0.;
        // Separate the accumulation of s from the multiplication
        for (int i = 0; i < LEN_1D; i++) {
            s += (float)2.;
            // Store intermediate s values in a[i] temporarily
            a[i] = s;
        }
        // Independent multiplication loop (easily vectorizable)
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = a[i] * b[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s453(int iterations, int LEN_1D, float* a, float *b) {
    typedef float v4f __attribute__((vector_size(16)));
    typedef int v4i __attribute__((vector_size(16)));

    const int VEC_SIZE = 4;
    const v4f vtwo = (v4f){2.0f, 2.0f, 2.0f, 2.0f};
    const v4i vstride = (v4i){0, 1, 2, 3};

    for (int nl = 0; nl < iterations*2; nl++) {
        float s = 0.0f;

        // Vectorized accumulation and store loop
        int i = 0;
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            // Generate s values for this vector: s + 2*[0,1,2,3]
            v4f s_vec = (v4f){s, s, s, s} + vtwo * (v4f)(vstride);

            // Store vector to a[i..i+3]
            v4f* a_vec = (v4f*)(&a[i]);
            *a_vec = s_vec;

            // Update scalar s for next iteration
            s += 2.0f * VEC_SIZE;
        }

        // Scalar tail for accumulation
        for (; i < LEN_1D; i++) {
            s += 2.0f;
            a[i] = s;
        }

        // Vectorized multiplication loop
        i = 0;
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            v4f* a_vec = (v4f*)(&a[i]);
            v4f* b_vec = (v4f*)(&b[i]);
            *a_vec = *a_vec * *b_vec;
        }

        // Scalar tail for multiplication
        for (; i < LEN_1D; i++) {
            a[i] = a[i] * b[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s453(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s453(iterations, LEN_1D, a_vector, b_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
