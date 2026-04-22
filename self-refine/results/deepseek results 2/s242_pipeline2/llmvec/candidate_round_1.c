#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    float s_sum = s1 + s2;
    int outer_iters = iterations / 5;

    for (int nl = 0; nl < outer_iters; nl++) {
        float prev_a = a[0];

        // Temporary array to store independent sums
        float* tmp = (float*)__builtin_alloca(LEN_1D * sizeof(float));

        // Vectorizable loop: compute independent sums
        for (int i = 1; i < LEN_1D; ++i) {
            tmp[i] = s_sum + b[i] + c[i] + d[i];
        }

        // Clean recurrence: only depends on previous a[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            prev_a = prev_a + tmp[i];
            a[i] = prev_a;
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    float s_sum = s1 + s2;
    int outer_iters = iterations / 5;

    for (int nl = 0; nl < outer_iters; nl++) {
        float prev_a = a[0];

        float* tmp = (float*)__builtin_alloca(LEN_1D * sizeof(float));

        // Vectorizable loop: compute independent sums using vector types
        typedef float float4 __attribute__((vector_size(16)));

        int i = 1;
        int vec_end = 1 + ((LEN_1D - 1) / 4) * 4;

        for (; i + 3 < LEN_1D; i += 4) {
            float4 vb, vc, vd, vr;
            memcpy(&vb, &b[i], 16);
            memcpy(&vc, &c[i], 16);
            memcpy(&vd, &d[i], 16);
            float4 vs = {s_sum, s_sum, s_sum, s_sum};
            vr = vs + vb + vc + vd;
            memcpy(&tmp[i], &vr, 16);
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            tmp[i] = s_sum + b[i] + c[i] + d[i];
        }

        // Recurrence: sequential prefix sum, cannot be vectorized directly
        for (int j = 1; j < LEN_1D; ++j) {
            prev_a = prev_a + tmp[j];
            a[j] = prev_a;
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
            int iterations = 5; int LEN_1D = arr_len; float s1 = 7; float s2 = 7; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s242(iterations, LEN_1D, s1, s2, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s242(iterations, LEN_1D, s1, s2, a_vector, b_vector, c_vector, d_vector);
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
