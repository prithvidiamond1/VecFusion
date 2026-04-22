#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s481(int iterations,int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: check condition and compute partial updates
        int should_return = 0;
        int last_safe = 0;

        for (int i = 0; i < LEN_1D; i++) {
            if (d[i] < (float)0.) {
                should_return = 1;
                last_safe = i;
                break;
            }
        }

        // Update only up to last_safe (or all if no negative found)
        int limit = should_return ? last_safe : LEN_1D;
        for (int i = 0; i < limit; i++) {
            a[i] += b[i] * c[i];
        }

        if (should_return) return;
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s481(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    typedef float v8f __attribute__((__vector_size__(32), __aligned__(1)));
    typedef int32_t v8i __attribute__((__vector_size__(32), __aligned__(1)));

    const int VF = 8;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized check for negative values in d
        int should_return = 0;
        int last_safe = 0;
        int i = 0;

        for (; i + VF <= LEN_1D; i += VF) {
            v8f dv = *(v8f*)&d[i];
            v8i mask = (v8i)(dv < (v8f){0,0,0,0,0,0,0,0});

            // Check if any lane has negative value
            int any_negative = 0;
            for (int lane = 0; lane < VF; lane++) {
                if (mask[lane]) {
                    any_negative = 1;
                    last_safe = i + lane;
                    break;
                }
            }

            if (any_negative) {
                should_return = 1;
                break;
            }
        }

        // Scalar tail for the check
        if (!should_return) {
            for (; i < LEN_1D; i++) {
                if (d[i] < 0.0f) {
                    should_return = 1;
                    last_safe = i;
                    break;
                }
            }
        }

        // Determine limit for update
        int limit = should_return ? last_safe : LEN_1D;

        // Vectorized update: a[i] += b[i] * c[i]
        i = 0;
        for (; i + VF <= limit; i += VF) {
            v8f av = *(v8f*)&a[i];
            v8f bv = *(v8f*)&b[i];
            v8f cv = *(v8f*)&c[i];
            av += bv * cv;
            *(v8f*)&a[i] = av;
        }

        // Scalar tail for update
        for (; i < limit; i++) {
            a[i] += b[i] * c[i];
        }

        if (should_return) return;
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
