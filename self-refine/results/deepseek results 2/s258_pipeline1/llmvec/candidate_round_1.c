#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a,float* b,float* c, float* d,float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.) {
                s = d[i] * d[i];
            }
            b[i] = s * c[i] + d[i];
            e[i] = (s + (float)1.) * aa[0][i];
        }
    }
}

        #include <stdint.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.f;
        // The loop has a carried dependency on s (conditional update),
        // so we must process it sequentially for correctness.
        // However, once s is determined, b[i] and e[i] can be computed with SIMD-friendly code.
        // Since s depends on a[i] conditionally, we first compute s in a scalar pass,
        // then vectorize the b[i] and e[i] computation.

        // Scalar pass to compute final s
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.f) {
                s = d[i] * d[i];
            }
        }

        // Now s is known; vectorize b[i] and e[i] computation
        // But wait - the original loop uses s as it evolves during iteration,
        // so b[i] and e[i] use the s value AT that point in the loop.
        // We cannot separate the passes without changing semantics.
        // We must keep the scalar loop for correctness.

        s = 0.f;
        int i = 0;
        // Process 8 at a time where possible, but s dependency forces scalar
        // We unroll manually to hint the compiler
        for (; i + 7 < 256; i += 8) {
            if (a[i+0] > 0.f) s = d[i+0] * d[i+0];
            b[i+0] = s * c[i+0] + d[i+0];
            e[i+0] = (s + 1.f) * aa[0][i+0];

            if (a[i+1] > 0.f) s = d[i+1] * d[i+1];
            b[i+1] = s * c[i+1] + d[i+1];
            e[i+1] = (s + 1.f) * aa[0][i+1];

            if (a[i+2] > 0.f) s = d[i+2] * d[i+2];
            b[i+2] = s * c[i+2] + d[i+2];
            e[i+2] = (s + 1.f) * aa[0][i+2];

            if (a[i+3] > 0.f) s = d[i+3] * d[i+3];
            b[i+3] = s * c[i+3] + d[i+3];
            e[i+3] = (s + 1.f) * aa[0][i+3];

            if (a[i+4] > 0.f) s = d[i+4] * d[i+4];
            b[i+4] = s * c[i+4] + d[i+4];
            e[i+4] = (s + 1.f) * aa[0][i+4];

            if (a[i+5] > 0.f) s = d[i+5] * d[i+5];
            b[i+5] = s * c[i+5] + d[i+5];
            e[i+5] = (s + 1.f) * aa[0][i+5];

            if (a[i+6] > 0.f) s = d[i+6] * d[i+6];
            b[i+6] = s * c[i+6] + d[i+6];
            e[i+6] = (s + 1.f) * aa[0][i+6];

            if (a[i+7] > 0.f) s = d[i+7] * d[i+7];
            b[i+7] = s * c[i+7] + d[i+7];
            e[i+7] = (s + 1.f) * aa[0][i+7];
        }
        for (; i < 256; ++i) {
            if (a[i] > 0.f) s = d[i] * d[i];
            b[i] = s * c[i] + d[i];
            e[i] = (s + 1.f) * aa[0][i];
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128]; float aa_scalar[128]; float aa_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                s258(iterations, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar, aa_scalar); vectorized_s258(iterations, a_vector, b_vector, c_vector, d_vector, e_vector, aa_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
