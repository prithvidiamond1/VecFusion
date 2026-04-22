#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;

        // First loop: compute s values and store them in a temporary array
        float s_vals[256];
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.) {
                s = d[i] * d[i];
            }
            s_vals[i] = s;
        }

        // Second loop: compute b[i] using precomputed s_vals
        for (int i = 0; i < 256; ++i) {
            b[i] = s_vals[i] * c[i] + d[i];
        }

        // Third loop: compute e[i] using precomputed s_vals
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_vals[i] + 1.0f) * aa[0][i];
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    const int N = 256;
    const int VEC_SIZE = 4;
    const int VEC_ITERS = N / VEC_SIZE;
    const int REMAINDER = N % VEC_SIZE;

    // Runtime alias detection
    int b_may_alias_a = (b >= a && b < a + N) || (a >= b && a < b + N);
    int b_may_alias_c = (b >= c && b < c + N) || (c >= b && c < b + N);
    int b_may_alias_d = (b >= d && b < d + N) || (d >= b && d < b + N);
    int b_may_alias_aa = (b >= &aa[0][0] && b < &aa[0][0] + N) || (&aa[0][0] >= b && &aa[0][0] < b + N);
    int b_has_alias = b_may_alias_a || b_may_alias_c || b_may_alias_d || b_may_alias_aa;

    int e_may_alias_a = (e >= a && e < a + N) || (a >= e && a < e + N);
    int e_may_alias_c = (e >= c && e < c + N) || (c >= e && c < e + N);
    int e_may_alias_d = (e >= d && e < d + N) || (d >= e && d < e + N);
    int e_may_alias_aa = (e >= &aa[0][0] && e < &aa[0][0] + N) || (&aa[0][0] >= e && &aa[0][0] < e + N);
    int e_has_alias = e_may_alias_a || e_may_alias_c || e_may_alias_d || e_may_alias_aa;

    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        float s_vals[N];

        // First loop: compute s values with cross-chunk dependency
        for (int i = 0; i < VEC_ITERS; i++) {
            int base = i * VEC_SIZE;
            float4 a_vec = *(float4*)&a[base];
            float4 d_vec = *(float4*)&d[base];

            // Vector comparison: a[i] > 0
            int4 mask = a_vec > (float4){0.0f, 0.0f, 0.0f, 0.0f};

            // Process elements sequentially to maintain dependency
            for (int j = 0; j < VEC_SIZE; j++) {
                if (mask[j]) {
                    s = d[base + j] * d[base + j];
                }
                s_vals[base + j] = s;
            }
        }

        // Handle remainder in first loop
        int base = VEC_ITERS * VEC_SIZE;
        for (int i = 0; i < REMAINDER; i++) {
            int idx = base + i;
            if (a[idx] > 0.0f) {
                s = d[idx] * d[idx];
            }
            s_vals[idx] = s;
        }

        // Second loop: alias-safe computation b[i] = s_vals[i] * c[i] + d[i]
        if (!b_has_alias) {
            // Vectorized version when no aliasing
            for (int i = 0; i < VEC_ITERS; i++) {
                int base = i * VEC_SIZE;
                float4 s_vec = *(float4*)&s_vals[base];
                float4 c_vec = *(float4*)&c[base];
                float4 d_vec = *(float4*)&d[base];

                float4 b_vec = s_vec * c_vec + d_vec;
                *(float4*)&b[base] = b_vec;
            }

            // Handle remainder in second loop
            base = VEC_ITERS * VEC_SIZE;
            for (int i = 0; i < REMAINDER; i++) {
                int idx = base + i;
                b[idx] = s_vals[idx] * c[idx] + d[idx];
            }
        } else {
            // Scalar fallback when aliasing detected
            for (int i = 0; i < N; i++) {
                b[i] = s_vals[i] * c[i] + d[i];
            }
        }

        // Third loop: alias-safe computation e[i] = (s_vals[i] + 1.0f) * aa[0][i]
        if (!e_has_alias) {
            // Vectorized version when no aliasing
            for (int i = 0; i < VEC_ITERS; i++) {
                int base = i * VEC_SIZE;
                float4 s_vec = *(float4*)&s_vals[base];
                float4 aa_vec = *(float4*)&aa[0][base];

                float4 e_vec = (s_vec + (float4){1.0f, 1.0f, 1.0f, 1.0f}) * aa_vec;
                *(float4*)&e[base] = e_vec;
            }

            // Handle remainder in third loop
            base = VEC_ITERS * VEC_SIZE;
            for (int i = 0; i < REMAINDER; i++) {
                int idx = base + i;
                e[idx] = (s_vals[idx] + 1.0f) * aa[0][idx];
            }
        } else {
            // Scalar fallback when aliasing detected
            for (int i = 0; i < N; i++) {
                e[i] = (s_vals[i] + 1.0f) * aa[0][i];
            }
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
