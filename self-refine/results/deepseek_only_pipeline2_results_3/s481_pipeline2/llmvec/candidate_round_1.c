#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s481(int iterations,int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int exit_flag = 0;
        int exit_index = LEN_1D;

        // First loop: check condition and find first negative
        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < (float)0.) {
                exit_flag = 1;
                exit_index = i;
                break;
            }
        }

        // Second loop: compute only up to exit_index
        for (int i = 0; i < exit_index; i++) {
            a[i] += b[i] * c[i];
        }

        if (exit_flag) {
            return;
        }
    }
}

        #include <stddef.h>

void vectorized_s481(int iterations, int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int exit_flag = 0;
        int exit_index = LEN_1D;

        // Vectorized search for first negative value
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        const int VF = 4;

        // Process vector chunks
        for (; i + VF <= LEN_1D; i += VF) {
            v4f dv = *(v4f*)&d[i];
            v4f mask = dv < (v4f){0.0f, 0.0f, 0.0f, 0.0f};

            // Check if any lane is negative
            int any_negative = 0;
            for (int lane = 0; lane < VF; lane++) {
                if (mask[lane]) {
                    any_negative = 1;
                    break;
                }
            }

            if (any_negative) {
                // Find first negative within this vector
                for (int lane = 0; lane < VF; lane++) {
                    if (d[i + lane] < 0.0f) {
                        exit_flag = 1;
                        exit_index = i + lane;
                        break;
                    }
                }
                break;
            }
        }

        // Scalar tail for search
        if (!exit_flag) {
            for (; i < LEN_1D; i++) {
                if (d[i] < 0.0f) {
                    exit_flag = 1;
                    exit_index = i;
                    break;
                }
            }
        }

        // Vectorized computation up to exit_index
        int j = 0;
        for (; j + VF <= exit_index; j += VF) {
            v4f av = *(v4f*)&a[j];
            v4f bv = *(v4f*)&b[j];
            v4f cv = *(v4f*)&c[j];
            av += bv * cv;
            *(v4f*)&a[j] = av;
        }

        // Scalar tail for computation
        for (; j < exit_index; j++) {
            a[j] += b[j] * c[j];
        }

        if (exit_flag) {
            return;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s481(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s481(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
