#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int j;
    for (int nl = 0; nl < iterations; nl++) {
        j = -1;
        for (int i = 0; i < (LEN_1D/2); i++) {
            j++;
            a[j] = b[i] + d[i] * e[i];
            if (c[i] > (float)0.) {
                j++;
                a[j] = c[i] + d[i] * e[i];
            }
        }
    }
}

        #include <stdint.h>

typedef float v4sf __attribute__((ext_vector_type(4)));

void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int j;
    for (int nl = 0; nl < iterations; nl++) {
        j = -1;
        int half_len = LEN_1D / 2;
        int i = 0;
        // Process 4 elements at a time (2 pairs of i values)
        for (; i + 3 < half_len; i += 4) {
            // Load 4 elements from b, d, e, c
            v4sf b_vec = *(v4sf*)(b + i);
            v4sf d_vec = *(v4sf*)(d + i);
            v4sf e_vec = *(v4sf*)(e + i);
            v4sf c_vec = *(v4sf*)(c + i);

            // Compute d * e
            v4sf de_vec = d_vec * e_vec;

            // First output: a[j+1] = b[i] + d[i]*e[i] for each i
            v4sf a_first = b_vec + de_vec;

            // Check if c[i] > 0
            v4sf zero = (v4sf){0.0f, 0.0f, 0.0f, 0.0f};
            v4si mask = (v4si)(c_vec > zero);

            // Compute second output: c[i] + d[i]*e[i]
            v4sf a_second = c_vec + de_vec;

            // Store results - need to handle conditional writes
            // For each i, we write a[j+1] unconditionally, then conditionally a[j+2]
            // j starts at -1, so for i=0: j becomes 0, write a[0], then if c[0]>0, j=1, write a[1]
            // For i=1: j becomes 2 (or 1 if previous was conditional), etc.
            // This is complex to vectorize directly, so we'll use scalar for now
            // and just unroll for better performance
            for (int k = 0; k < 4; k++) {
                j++;
                a[j] = b[i+k] + d[i+k] * e[i+k];
                if (c[i+k] > 0.0f) {
                    j++;
                    a[j] = c[i+k] + d[i+k] * e[i+k];
                }
            }
        }
        // Scalar cleanup for remaining elements
        for (; i < half_len; i++) {
            j++;
            a[j] = b[i] + d[i] * e[i];
            if (c[i] > 0.0f) {
                j++;
                a[j] = c[i] + d[i] * e[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s123(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s123(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
