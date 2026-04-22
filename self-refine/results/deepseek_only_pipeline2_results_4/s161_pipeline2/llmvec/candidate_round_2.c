#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // First pass: compute all c[i+1] updates for b[i] < 0
        // Use original a[i] values before any updates in this iteration
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] < (float)0.) {
                c[i+1] = a[i] + d[i] * d[i];
            }
        }

        // Second pass: compute all a[i] updates for b[i] >= 0
        // Use updated c[i] values from first pass when needed
        for (int i = 0; i < LEN_1D; ++i) {
            if (b[i] >= (float)0.) {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    typedef float v4f __attribute__((vector_size(16)));
    typedef int v4i __attribute__((vector_size(16)));

    int outer_loop_count = iterations / 2;

    for (int nl = 0; nl < outer_loop_count; nl++) {
        // First pass: compute c[i+1] for b[i] < 0
        int i = 0;
        for (; i + 3 < LEN_1D - 1; i += 4) {
            v4f b_vec = *(v4f*)&b[i];
            v4f zero_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};
            v4i mask_vec = b_vec < zero_vec;

            // Convert vector mask to scalar mask
            int mask = (mask_vec[0] ? 1 : 0) |
                       (mask_vec[1] ? 2 : 0) |
                       (mask_vec[2] ? 4 : 0) |
                       (mask_vec[3] ? 8 : 0);

            if (mask != 0) {
                v4f a_vec = *(v4f*)&a[i];
                v4f d_vec = *(v4f*)&d[i];
                v4f d_sq_vec = d_vec * d_vec;
                v4f result_vec = a_vec + d_sq_vec;

                // Store results for active lanes only
                for (int lane = 0; lane < 4; lane++) {
                    if (mask & (1 << lane)) {
                        c[i + lane + 1] = result_vec[lane];
                    }
                }
            }
        }

        // Scalar tail for first pass
        for (; i < LEN_1D - 1; ++i) {
            if (b[i] < 0.0f) {
                c[i + 1] = a[i] + d[i] * d[i];
            }
        }

        // Second pass: compute a[i] for b[i] >= 0
        i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            v4f b_vec = *(v4f*)&b[i];
            v4f zero_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};
            v4i mask_vec = b_vec >= zero_vec;

            // Convert vector mask to scalar mask
            int mask = (mask_vec[0] ? 1 : 0) |
                       (mask_vec[1] ? 2 : 0) |
                       (mask_vec[2] ? 4 : 0) |
                       (mask_vec[3] ? 8 : 0);

            if (mask != 0) {
                v4f c_vec = *(v4f*)&c[i];
                v4f d_vec = *(v4f*)&d[i];
                v4f e_vec = *(v4f*)&e[i];
                v4f de_vec = d_vec * e_vec;
                v4f result_vec = c_vec + de_vec;

                // Store results for active lanes only
                for (int lane = 0; lane < 4; lane++) {
                    if (mask & (1 << lane)) {
                        a[i + lane] = result_vec[lane];
                    }
                }
            }
        }

        // Scalar tail for second pass
        for (; i < LEN_1D; ++i) {
            if (b[i] >= 0.0f) {
                a[i] = c[i] + d[i] * e[i];
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
                s161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
