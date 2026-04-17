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
        max = fabsf(a[0]);
        for (int i = 0; i < LEN_1D; i++) {
            if ((fabsf(a[i])) > max) {
                max = fabsf(a[i]);
            }
        }
    }
    return max;
}

        #include <math.h>

float vectorized_s3113(int iterations, int LEN_1D, float* a)
{
    float max;
    for (int nl = 0; nl < iterations * 4; nl++) {
        // Process 8 elements at a time using unrolled vector-friendly code
        int i = 0;
        float max0 = fabsf(a[0]);
        float max1 = max0;
        float max2 = max0;
        float max3 = max0;
        float max4 = max0;
        float max5 = max0;
        float max6 = max0;
        float max7 = max0;

        int limit = LEN_1D - (LEN_1D % 8);
        for (i = 0; i < limit; i += 8) {
            float v0 = fabsf(a[i + 0]);
            float v1 = fabsf(a[i + 1]);
            float v2 = fabsf(a[i + 2]);
            float v3 = fabsf(a[i + 3]);
            float v4 = fabsf(a[i + 4]);
            float v5 = fabsf(a[i + 5]);
            float v6 = fabsf(a[i + 6]);
            float v7 = fabsf(a[i + 7]);
            if (v0 > max0) max0 = v0;
            if (v1 > max1) max1 = v1;
            if (v2 > max2) max2 = v2;
            if (v3 > max3) max3 = v3;
            if (v4 > max4) max4 = v4;
            if (v5 > max5) max5 = v5;
            if (v6 > max6) max6 = v6;
            if (v7 > max7) max7 = v7;
        }

        // Reduce the 8 partial maxima
        if (max1 > max0) max0 = max1;
        if (max2 > max0) max0 = max2;
        if (max3 > max0) max0 = max3;
        if (max4 > max0) max0 = max4;
        if (max5 > max0) max0 = max5;
        if (max6 > max0) max0 = max6;
        if (max7 > max0) max0 = max7;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float v = fabsf(a[i]);
            if (v > max0) max0 = v;
        }

        max = max0;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s3113(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s3113(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
