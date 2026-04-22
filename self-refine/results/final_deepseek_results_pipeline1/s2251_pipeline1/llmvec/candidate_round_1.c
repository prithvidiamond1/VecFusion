#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2251(int iterations, int LEN_1D, float* a,float* b,float* c,float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = s*e[i];
            s = b[i]+c[i];
            b[i] = a[i]+d[i];
        }
    }
}

        #include <stddef.h>

typedef float float32_t __attribute__((ext_vector_type(4)));

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        int i = 0;

        // Vectorized main loop
        for (; i <= LEN_1D - 4; i += 4) {
            float32_t vec_e = *(float32_t*)&e[i];
            float32_t vec_s = (float32_t){s, s, s, s};

            // a[i] = s * e[i]
            float32_t vec_a = vec_s * vec_e;
            *(float32_t*)&a[i] = vec_a;

            // s = b[i] + c[i]
            float32_t vec_b = *(float32_t*)&b[i];
            float32_t vec_c = *(float32_t*)&c[i];
            float32_t vec_s_new = vec_b + vec_c;

            // b[i] = a[i] + d[i]
            float32_t vec_d = *(float32_t*)&d[i];
            float32_t vec_b_new = vec_a + vec_d;
            *(float32_t*)&b[i] = vec_b_new;

            // Update s for next iteration (last element of vec_s_new)
            s = vec_s_new[3];
        }

        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] = s * e[i];
            s = b[i] + c[i];
            b[i] = a[i] + d[i];
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
                s2251(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s2251(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
