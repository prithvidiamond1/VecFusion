#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s315(int iterations,int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x, chksum;
    int index;
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;
        for (int i = 0; i < LEN_1D; ++i) {
            if (a[i] > x) {
                x = a[i];
                index = i;
            }
        }
        chksum = x + (float) index;
    }
    return index + x + 1;
}

        #include <stdint.h>
#include <float.h>

float vectorized_s315(int iterations, int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x, chksum;
    int index;
    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized max-finding with 4-wide unroll
        float max_val0 = -FLT_MAX, max_val1 = -FLT_MAX;
        float max_val2 = -FLT_MAX, max_val3 = -FLT_MAX;
        int max_idx0 = 0, max_idx1 = 0, max_idx2 = 0, max_idx3 = 0;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            float v0 = a[i+0];
            float v1 = a[i+1];
            float v2 = a[i+2];
            float v3 = a[i+3];

            if (v0 > max_val0) { max_val0 = v0; max_idx0 = i+0; }
            if (v1 > max_val1) { max_val1 = v1; max_idx1 = i+1; }
            if (v2 > max_val2) { max_val2 = v2; max_idx2 = i+2; }
            if (v3 > max_val3) { max_val3 = v3; max_idx3 = i+3; }
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            if (a[i] > max_val0) { max_val0 = a[i]; max_idx0 = i; }
        }

        // Reduce across 4 lanes
        // Merge lane1 into lane0
        if (max_val1 > max_val0) { max_val0 = max_val1; max_idx0 = max_idx1; }
        // Merge lane2 into lane0
        if (max_val2 > max_val0) { max_val0 = max_val2; max_idx0 = max_idx2; }
        // Merge lane3 into lane0
        if (max_val3 > max_val0) { max_val0 = max_val3; max_idx0 = max_idx3; }

        x = max_val0;
        index = max_idx0;
        chksum = x + (float)index;
    }
    return index + x + 1;
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
                ret_scalar = s315(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s315(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
