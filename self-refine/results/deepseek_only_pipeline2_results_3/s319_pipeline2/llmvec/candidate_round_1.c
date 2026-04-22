#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s319(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e) {
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        float sum_a = 0.;
        float sum_b = 0.;

        // First loop: compute a[i] and accumulate its sum
        for (int i = 0; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            a[i] = tmp_a;
            sum_a += tmp_a;
        }

        // Second loop: compute b[i] and accumulate its sum
        for (int i = 0; i < LEN_1D; i++) {
            float tmp_b = c[i] + e[i];
            b[i] = tmp_b;
            sum_b += tmp_b;
        }

        // Combine the partial sums
        sum = sum_a + sum_b;
    }
    return sum;
}

        #include <stddef.h>

float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    float sum = 0.0f;

    for (int nl = 0; nl < 2 * iterations; nl++) {
        float sum_a = 0.0f;
        float sum_b = 0.0f;

        // Vectorized first loop
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec = *(v4f *)&c[i];
            v4f d_vec = *(v4f *)&d[i];
            v4f tmp_a_vec = c_vec + d_vec;
            *(v4f *)&a[i] = tmp_a_vec;

            // Horizontal sum of vector
            float tmp_arr[4];
            *(v4f *)tmp_arr = tmp_a_vec;
            sum_a += tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3];
        }

        // Scalar tail for first loop
        for (; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            a[i] = tmp_a;
            sum_a += tmp_a;
        }

        // Vectorized second loop
        i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec = *(v4f *)&c[i];
            v4f e_vec = *(v4f *)&e[i];
            v4f tmp_b_vec = c_vec + e_vec;
            *(v4f *)&b[i] = tmp_b_vec;

            // Horizontal sum of vector
            float tmp_arr[4];
            *(v4f *)tmp_arr = tmp_b_vec;
            sum_b += tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3];
        }

        // Scalar tail for second loop
        for (; i < LEN_1D; i++) {
            float tmp_b = c[i] + e[i];
            b[i] = tmp_b;
            sum_b += tmp_b;
        }

        sum = sum_a + sum_b;
    }

    return sum;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                ret_scalar = s319(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); ret_vector = vectorized_s319(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
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
