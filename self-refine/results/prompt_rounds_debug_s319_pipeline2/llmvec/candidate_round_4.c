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
    float total_sum = 0.0f;
    int outer_iterations = 2 * iterations;

    for (int nl = 0; nl < outer_iterations; nl++) {
        float sum = 0.0f;

        if (LEN_1D <= 0) {
            total_sum += sum;
            continue;
        }

        // Scalar prologue to align all vector accesses
        int i = 0;
        // Compute misalignment based on the first array we'll vector-load from (c)
        uintptr_t c_ptr = (uintptr_t)&c[0];
        int misalign = c_ptr % 16;
        if (misalign != 0) {
            int floats_to_align = (16 - misalign) / sizeof(float);
            if (floats_to_align > LEN_1D) {
                floats_to_align = LEN_1D;
            }
            for (; i < floats_to_align; i++) {
                float tmp_a = c[i] + d[i];
                float tmp_b = c[i] + e[i];
                a[i] = tmp_a;
                b[i] = tmp_b;
                sum += tmp_a + tmp_b;
            }
        }

        // Vectorized main loop
        int vec_limit = i + ((LEN_1D - i) / 4) * 4;
        float4 sum_vec = (float4){0.0f, 0.0f, 0.0f, 0.0f};

        for (; i < vec_limit; i += 4) {
            // Use explicit unaligned loads to avoid alignment requirements
            float4 c_vec;
            float4 d_vec;
            float4 e_vec;
            float *c_base = &c[i];
            float *d_base = &d[i];
            float *e_base = &e[i];
            c_vec.x = c_base[0]; c_vec.y = c_base[1]; c_vec.z = c_base[2]; c_vec.w = c_base[3];
            d_vec.x = d_base[0]; d_vec.y = d_base[1]; d_vec.z = d_base[2]; d_vec.w = d_base[3];
            e_vec.x = e_base[0]; e_vec.y = e_base[1]; e_vec.z = e_base[2]; e_vec.w = e_base[3];

            float4 tmp_a_vec = c_vec + d_vec;
            float4 tmp_b_vec = c_vec + e_vec;

            // Store results
            a[i] = tmp_a_vec.x; a[i+1] = tmp_a_vec.y; a[i+2] = tmp_a_vec.z; a[i+3] = tmp_a_vec.w;
            b[i] = tmp_b_vec.x; b[i+1] = tmp_b_vec.y; b[i+2] = tmp_b_vec.z; b[i+3] = tmp_b_vec.w;

            float4 lane_sum = tmp_a_vec + tmp_b_vec;
            sum_vec += lane_sum;
        }

        // Horizontal reduction
        sum += sum_vec.x + sum_vec.y + sum_vec.z + sum_vec.w;

        // Scalar epilogue
        for (; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            float tmp_b = c[i] + e[i];
            a[i] = tmp_a;
            b[i] = tmp_b;
            sum += tmp_a + tmp_b;
        }

        total_sum += sum;
    }

    return total_sum;
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
