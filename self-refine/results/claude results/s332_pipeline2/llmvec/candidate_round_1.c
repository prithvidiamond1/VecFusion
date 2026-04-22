#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s332(int iterations, int LEN_1D, int t, float* a)
{
    int index;
    float value;
    float chksum;

    // Temporary mask array for phase 1 (vectorizable comparison)
    int mask[LEN_1D];

    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.f;

        // Phase 1: Vectorizable comparison pass
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (a[i] > t) ? 1 : 0;
        }

        // Phase 2: Scalar scan to find first match
        for (int i = 0; i < LEN_1D; i++) {
            if (mask[i]) {
                index = i;
                value = a[i];
                break;
            }
        }

        chksum = value + (float)index;
    }
    return value;
}

        #include <stdint.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a)
{
    int index;
    float value;
    float chksum;

    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.f;

        // Phase 1 + Phase 2 combined with SIMD-friendly unrolling
        // Process 8 elements at a time, check mask, find first match
        int found = 0;
        int i = 0;

        // Unrolled loop: process 8 elements per iteration
        for (; i <= LEN_1D - 8 && !found; i += 8) {
            int m0 = (a[i+0] > (float)t) ? 1 : 0;
            int m1 = (a[i+1] > (float)t) ? 1 : 0;
            int m2 = (a[i+2] > (float)t) ? 1 : 0;
            int m3 = (a[i+3] > (float)t) ? 1 : 0;
            int m4 = (a[i+4] > (float)t) ? 1 : 0;
            int m5 = (a[i+5] > (float)t) ? 1 : 0;
            int m6 = (a[i+6] > (float)t) ? 1 : 0;
            int m7 = (a[i+7] > (float)t) ? 1 : 0;

            if (m0 | m1 | m2 | m3 | m4 | m5 | m6 | m7) {
                // Find first match in this group
                if (m0) { index = i+0; value = a[i+0]; found = 1; }
                else if (m1) { index = i+1; value = a[i+1]; found = 1; }
                else if (m2) { index = i+2; value = a[i+2]; found = 1; }
                else if (m3) { index = i+3; value = a[i+3]; found = 1; }
                else if (m4) { index = i+4; value = a[i+4]; found = 1; }
                else if (m5) { index = i+5; value = a[i+5]; found = 1; }
                else if (m6) { index = i+6; value = a[i+6]; found = 1; }
                else if (m7) { index = i+7; value = a[i+7]; found = 1; }
            }
        }

        // Scalar cleanup tail
        for (; i < LEN_1D && !found; i++) {
            if (a[i] > (float)t) {
                index = i;
                value = a[i];
                found = 1;
            }
        }

        chksum = value + (float)index;
    }
    return value;
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
            int iterations = 5; int LEN_1D = n; int t = 7; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s332(iterations, LEN_1D, t, a_scalar); ret_vector = vectorized_s332(iterations, LEN_1D, t, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
