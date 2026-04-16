#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    // The loop-carried dependency: c[i+1] written in iteration i is read as c[i] in iteration i+1
    // when b[i] < 0. We need to preserve original c values before updating.
    // Use a temporary array to store original c values to break the dependency.

    float* c_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations/2; nl++) {
        // Save original c values before the loop modifies them
        memcpy(c_orig, c, LEN_1D * sizeof(float));

        // Branchless version:
        // If b[i] >= 0: a[i] = c[i] + d[i]*e[i], c[i+1] unchanged
        // If b[i] < 0:  c[i+1] = a[i] + d[i]*d[i], a[i] unchanged
        // Use c_orig[i] for the read of c[i] to avoid loop-carried dependency

        for (int i = 0; i < LEN_1D-1; ++i) {
            int neg = (b[i] < (float)0.);
            int pos = 1 - neg;
            a[i]   = pos * (c_orig[i] + d[i] * e[i]) + neg * a[i];
            c[i+1] = neg * (a[i] + d[i] * d[i])      + pos * c[i+1];
        }
    }
}

        #include <string.h>
#include <stdint.h>

void vectorized_s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    float* c_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations/2; nl++) {
        memcpy(c_orig, c, LEN_1D * sizeof(float));

        int i = 0;
        int limit = LEN_1D - 1;

#if defined(__clang__) || defined(__GNUC__)
        typedef float float4 __attribute__((vector_size(16)));

        int limit4 = limit - (limit % 4);
        for (; i < limit4; i += 4) {
            float4 b_vec  = { b[i], b[i+1], b[i+2], b[i+3] };
            float4 zero   = { 0.f, 0.f, 0.f, 0.f };
            float4 one    = { 1.f, 1.f, 1.f, 1.f };

            // neg mask: b[i] < 0 => -1 (all bits set), else 0
            // We'll compute neg as float 1.0 or 0.0 manually
            float neg0 = (b[i]   < 0.f) ? 1.f : 0.f;
            float neg1 = (b[i+1] < 0.f) ? 1.f : 0.f;
            float neg2 = (b[i+2] < 0.f) ? 1.f : 0.f;
            float neg3 = (b[i+3] < 0.f) ? 1.f : 0.f;

            float pos0 = 1.f - neg0;
            float pos1 = 1.f - neg1;
            float pos2 = 1.f - neg2;
            float pos3 = 1.f - neg3;

            float4 neg_vec = { neg0, neg1, neg2, neg3 };
            float4 pos_vec = { pos0, pos1, pos2, pos3 };

            float4 c_orig_vec = { c_orig[i], c_orig[i+1], c_orig[i+2], c_orig[i+3] };
            float4 d_vec      = { d[i], d[i+1], d[i+2], d[i+3] };
            float4 e_vec      = { e[i], e[i+1], e[i+2], e[i+3] };
            float4 a_vec      = { a[i], a[i+1], a[i+2], a[i+3] };
            float4 c1_vec     = { c[i+1], c[i+2], c[i+3], c[i+4] };

            // a[i] = pos * (c_orig[i] + d[i]*e[i]) + neg * a[i]
            float4 new_a = pos_vec * (c_orig_vec + d_vec * e_vec) + neg_vec * a_vec;

            // c[i+1] = neg * (a[i] + d[i]*d[i]) + pos * c[i+1]
            // Note: use original a[i] (a_vec) here, not new_a
            float4 new_c1 = neg_vec * (a_vec + d_vec * d_vec) + pos_vec * c1_vec;

            a[i]   = new_a[0]; a[i+1] = new_a[1]; a[i+2] = new_a[2]; a[i+3] = new_a[3];
            c[i+1] = new_c1[0]; c[i+2] = new_c1[1]; c[i+3] = new_c1[2]; c[i+4] = new_c1[3];
        }
#endif

        // Scalar tail
        for (; i < limit; i++) {
            int neg = (b[i] < 0.f);
            int pos = 1 - neg;
            float a_old = a[i];
            a[i]   = pos * (c_orig[i] + d[i] * e[i]) + neg * a_old;
            c[i+1] = neg * (a_old + d[i] * d[i])     + pos * c[i+1];
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
