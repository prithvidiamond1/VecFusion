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

        float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    float sum = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        float sum_a = 0.0f;
        float sum_b = 0.0f;

        // First loop: compute a[i] = c[i] + d[i] and accumulate sum_a
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        v4f sum_a_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};

        // Vectorized part for a[i]
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec, d_vec;
            __builtin_memcpy(&c_vec, &c[i], 16);
            __builtin_memcpy(&d_vec, &d[i], 16);
            v4f tmp_a_vec = c_vec + d_vec;

            __builtin_memcpy(&a[i], &tmp_a_vec, 16);
            sum_a_vec += tmp_a_vec;
        }

        // Reduce vector sum for sum_a
        float sum_a_vec_arr[4];
        __builtin_memcpy(sum_a_vec_arr, &sum_a_vec, 16);
        sum_a = sum_a_vec_arr[0] + sum_a_vec_arr[1] + sum_a_vec_arr[2] + sum_a_vec_arr[3];

        // Scalar tail for a[i]
        for (; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            a[i] = tmp_a;
            sum_a += tmp_a;
        }

        // Second loop: compute b[i] = c[i] + (c[i] < 0 ? d[i] : e[i]) and accumulate sum_b
        i = 0;
        v4f sum_b_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};

        // Vectorized part for b[i] with conditional selection
        for (; i + 3 < LEN_1D; i += 4) {
            v4f c_vec, d_vec, e_vec;
            __builtin_memcpy(&c_vec, &c[i], 16);
            __builtin_memcpy(&d_vec, &d[i], 16);
            __builtin_memcpy(&e_vec, &e[i], 16);

            // Create mask for c_vec < 0
            v4f mask = __builtin_convertvector(c_vec < (v4f){0.0f, 0.0f, 0.0f, 0.0f}, v4f);

            // Select between d_vec and e_vec based on mask
            v4f selected_vec = (mask != (v4f){0.0f, 0.0f, 0.0f, 0.0f}) ? d_vec : e_vec;

            // Compute b[i] = c[i] + selected_vec
            v4f tmp_b_vec = c_vec + selected_vec;

            __builtin_memcpy(&b[i], &tmp_b_vec, 16);
            sum_b_vec += tmp_b_vec;
        }

        // Reduce vector sum for sum_b
        float sum_b_vec_arr[4];
        __builtin_memcpy(sum_b_vec_arr, &sum_b_vec, 16);
        sum_b = sum_b_vec_arr[0] + sum_b_vec_arr[1] + sum_b_vec_arr[2] + sum_b_vec_arr[3];

        // Scalar tail for b[i] with conditional
        for (; i < LEN_1D; i++) {
            float selected = (c[i] < 0.0f) ? d[i] : e[i];
            float tmp_b = c[i] + selected;
            b[i] = tmp_b;
            sum_b += tmp_b;
        }

        // Accumulate across outer iterations
        sum += sum_a + sum_b;
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
