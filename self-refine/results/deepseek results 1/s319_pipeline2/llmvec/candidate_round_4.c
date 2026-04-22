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
        for (int i = 0; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            float tmp_b = c[i] + e[i];
            a[i] = tmp_a;
            b[i] = tmp_b;
            sum += tmp_a;
            sum += tmp_b;
        }
    }
    return sum;
}

        #include <stdint.h>

typedef float float8 __attribute__((vector_size(32)));

float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e)
{
    float sum = 0.0f;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        sum = 0.0f;

        float8 vsum = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

        int vec_len = LEN_1D & ~7;
        int i = 0;

        for (; i < vec_len; i += 8) {
            float8 vc, vd, ve, vtmp_a, vtmp_b;

            vc = (float8){c[i], c[i+1], c[i+2], c[i+3], c[i+4], c[i+5], c[i+6], c[i+7]};
            vd = (float8){d[i], d[i+1], d[i+2], d[i+3], d[i+4], d[i+5], d[i+6], d[i+7]};
            ve = (float8){e[i], e[i+1], e[i+2], e[i+3], e[i+4], e[i+5], e[i+6], e[i+7]};

            vtmp_a = vc + vd;
            vtmp_b = vc + ve;

            a[i]   = vtmp_a[0]; a[i+1] = vtmp_a[1]; a[i+2] = vtmp_a[2]; a[i+3] = vtmp_a[3];
            a[i+4] = vtmp_a[4]; a[i+5] = vtmp_a[5]; a[i+6] = vtmp_a[6]; a[i+7] = vtmp_a[7];

            b[i]   = vtmp_b[0]; b[i+1] = vtmp_b[1]; b[i+2] = vtmp_b[2]; b[i+3] = vtmp_b[3];
            b[i+4] = vtmp_b[4]; b[i+5] = vtmp_b[5]; b[i+6] = vtmp_b[6]; b[i+7] = vtmp_b[7];

            vsum += vtmp_a;
        }

        sum += vsum[0] + vsum[1] + vsum[2] + vsum[3] +
               vsum[4] + vsum[5] + vsum[6] + vsum[7];

        for (; i < LEN_1D; i++) {
            float tmp_a = c[i] + d[i];
            float tmp_b = c[i] + e[i];
            a[i] = tmp_a;
            b[i] = tmp_b;
            sum += tmp_a;
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
