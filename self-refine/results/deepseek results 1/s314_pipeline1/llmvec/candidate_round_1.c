#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s314(int iterations,int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > x) {
                x = a[i];
            }
        }
    }
    return x;
}

        #include <stddef.h>

float vectorized_s314(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        // Use 4-wide unrolling to find maximum
        float x0, x1, x2, x3;
        x0 = a[0];
        x1 = a[0];
        x2 = a[0];
        x3 = a[0];

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            float v0 = a[i + 0];
            float v1 = a[i + 1];
            float v2 = a[i + 2];
            float v3 = a[i + 3];
            if (v0 > x0) x0 = v0;
            if (v1 > x1) x1 = v1;
            if (v2 > x2) x2 = v2;
            if (v3 > x3) x3 = v3;
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            if (a[i] > x0) x0 = a[i];
        }
        // Reduce across lanes
        if (x1 > x0) x0 = x1;
        if (x2 > x0) x0 = x2;
        if (x3 > x0) x0 = x3;
        x = x0;
    }
    return x;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s314(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s314(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
