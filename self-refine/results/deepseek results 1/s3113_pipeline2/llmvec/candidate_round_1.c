#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s3113(int iterations,int LEN_1D, float* a)
{
    float max;
    for (int nl = 0; nl < iterations*4; nl++) {
        float current_max = fabsf(a[0]);
        int i;

        // Process first element separately to simplify loop
        for (i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[i]);
            if (abs_val > current_max) {
                current_max = abs_val;
            }
        }

        max = current_max;
    }
    return max;
}

        #include <math.h>

float vectorized_s3113(int iterations, int LEN_1D, float* a)
{
    float max;
    for (int nl = 0; nl < iterations * 4; nl++) {
        float current_max0 = 0.0f;
        float current_max1 = 0.0f;
        float current_max2 = 0.0f;
        float current_max3 = 0.0f;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);

        for (i = 0; i < limit; i += 4) {
            float v0 = fabsf(a[i + 0]);
            float v1 = fabsf(a[i + 1]);
            float v2 = fabsf(a[i + 2]);
            float v3 = fabsf(a[i + 3]);
            if (v0 > current_max0) current_max0 = v0;
            if (v1 > current_max1) current_max1 = v1;
            if (v2 > current_max2) current_max2 = v2;
            if (v3 > current_max3) current_max3 = v3;
        }

        // Reduce the 4 accumulators
        if (current_max1 > current_max0) current_max0 = current_max1;
        if (current_max2 > current_max0) current_max0 = current_max2;
        if (current_max3 > current_max0) current_max0 = current_max3;

        // Scalar tail
        for (; i < LEN_1D; i++) {
            float v = fabsf(a[i]);
            if (v > current_max0) current_max0 = v;
        }

        max = current_max0;
    }
    return max;
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
                ret_scalar = s3113(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s3113(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
