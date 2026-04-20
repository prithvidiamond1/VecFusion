#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s319(int iterations,int LEN_1D, float * a, float*b,float*c,float*d, float*e)
{
    float sum;
    for (int nl = 0; nl < 2*iterations; nl++) {
        sum = 0.;
        // Compute and accumulate in one pass with temporaries
        for (int i = 0; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            float tmp_b = c[i] + e[i];
            a[i] = tmp_a;
            b[i] = tmp_b;
            sum += tmp_a + tmp_b;
        }
    }
    return sum;
}

        typedef float float4 __attribute__((ext_vector_type(4)));

float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    float sum;
    int outer_iterations = 2 * iterations;

    for (int nl = 0; nl < outer_iterations; nl++) {
        sum = 0.0f;

        if (LEN_1D <= 0) {
            continue;
        }

        // Handle misaligned start with scalar prologue
        int i = 0;
        int misalign = ((uintptr_t)&c[0]) % 16;
        if (misalign != 0) {
            int prologue_end = (4 - (misalign / 4)) % 4;
            if (prologue_end > LEN_1D) {
                prologue_end = LEN_1D;
            }
            for (; i < prologue_end; i++) {
                float tmp_a = c[i] + d[i];
                float tmp_b = c[i] + e[i];
                a[i] = tmp_a;
                b[i] = tmp_b;
                sum += tmp_a + tmp_b;
            }
        }

        // Vectorized main loop with aligned accesses
        int vec_limit = i + ((LEN_1D - i) / 4) * 4;
        float4 sum_vec = (float4){0.0f, 0.0f, 0.0f, 0.0f};

        for (; i < vec_limit; i += 4) {
            float4 c_vec = *(float4 *)&c[i];
            float4 d_vec = *(float4 *)&d[i];
            float4 e_vec = *(float4 *)&e[i];

            float4 tmp_a_vec = c_vec + d_vec;
            float4 tmp_b_vec = c_vec + e_vec;

            *(float4 *)&a[i] = tmp_a_vec;
            *(float4 *)&b[i] = tmp_b_vec;

            // Accumulate in same order as scalar: (c+d) + (c+e) for each element
            float4 lane_sum = tmp_a_vec + tmp_b_vec;
            sum_vec += lane_sum;
        }

        // Horizontal reduction preserving order
        float temp_sum = sum;
        temp_sum += sum_vec.x;
        temp_sum += sum_vec.y;
        temp_sum += sum_vec.z;
        temp_sum += sum_vec.w;
        sum = temp_sum;

        // Scalar epilogue for remaining elements
        for (; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            float tmp_b = c[i] + e[i];
            a[i] = tmp_a;
            b[i] = tmp_b;
            sum += tmp_a + tmp_b;
        }
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
