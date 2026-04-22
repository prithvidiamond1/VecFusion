#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s316(int iterations,int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            if (a[i] < x) {
                x = a[i];
            }
        }
    }
    return x;
}

        #include <float.h>

float vectorized_s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        x = a[0];

        // Process 8 elements at a time using unrolled scalar (vector-friendly)
        int i = 1;
        float min0 = x, min1 = x, min2 = x, min3 = x;
        float min4 = x, min5 = x, min6 = x, min7 = x;

        for (; i <= LEN_1D - 8; i += 8) {
            if (a[i+0] < min0) min0 = a[i+0];
            if (a[i+1] < min1) min1 = a[i+1];
            if (a[i+2] < min2) min2 = a[i+2];
            if (a[i+3] < min3) min3 = a[i+3];
            if (a[i+4] < min4) min4 = a[i+4];
            if (a[i+5] < min5) min5 = a[i+5];
            if (a[i+6] < min6) min6 = a[i+6];
            if (a[i+7] < min7) min7 = a[i+7];
        }

        // Reduce the 8 partial minimums
        if (min1 < min0) min0 = min1;
        if (min2 < min0) min0 = min2;
        if (min3 < min0) min0 = min3;
        if (min4 < min0) min0 = min4;
        if (min5 < min0) min0 = min5;
        if (min6 < min0) min0 = min6;
        if (min7 < min0) min0 = min7;
        x = min0;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            if (a[i] < x) {
                x = a[i];
            }
        }
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
                ret_scalar = s316(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s316(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
