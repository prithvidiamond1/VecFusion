#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float min_val = a[0];

        // Main vectorizable reduction loop
        for (int i = 1; i < LEN_1D; ++i) {
            float temp = a[i];
            if (temp < min_val) {
                min_val = temp;
            }
        }

        x = min_val;
    }
    return x;
}

        #include <float.h>

float vectorized_s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        float min_val = a[0];

        // Unrolled reduction with 8-wide unrolling
        int i = 1;
        int limit = LEN_1D - 7;

        float m0, m1, m2, m3, m4, m5, m6, m7;
        m0 = m1 = m2 = m3 = m4 = m5 = m6 = m7 = min_val;

        for (; i < limit; i += 8) {
            float v0 = a[i + 0];
            float v1 = a[i + 1];
            float v2 = a[i + 2];
            float v3 = a[i + 3];
            float v4 = a[i + 4];
            float v5 = a[i + 5];
            float v6 = a[i + 6];
            float v7 = a[i + 7];

            if (v0 < m0) m0 = v0;
            if (v1 < m1) m1 = v1;
            if (v2 < m2) m2 = v2;
            if (v3 < m3) m3 = v3;
            if (v4 < m4) m4 = v4;
            if (v5 < m5) m5 = v5;
            if (v6 < m6) m6 = v6;
            if (v7 < m7) m7 = v7;
        }

        // Reduce the 8 accumulators
        if (m1 < m0) m0 = m1;
        if (m2 < m0) m0 = m2;
        if (m3 < m0) m0 = m3;
        if (m4 < m0) m0 = m4;
        if (m5 < m0) m0 = m5;
        if (m6 < m0) m0 = m6;
        if (m7 < m0) m0 = m7;

        min_val = m0;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float temp = a[i];
            if (temp < min_val) {
                min_val = temp;
            }
        }

        x = min_val;
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
