#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s315(int iterations, int LEN_1D, float* a) {
    // First loop: fill array (already vectorizable)
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x, chksum;
    int index;

    // Main iterations loop
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // Loop distribution: separate max-value and first-index finding
        // First find max value (vectorizable reduction)
        float max_val = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            if (a[i] > max_val) {
                max_val = a[i];
            }
        }

        // Then find first index with max value (vectorizable comparison)
        // Use conditional assignment without branching for first occurrence
        int first_index = 0;
        for (int i = 0; i < LEN_1D; ++i) {
            // Update index only when value equals max_val AND first_index hasn't been set yet
            // This maintains "first occurrence" semantics
            first_index = (a[i] == max_val && first_index == 0) ? i : first_index;
        }

        x = max_val;
        index = first_index;
        chksum = x + (float)index;
    }

    return index + x + 1;
}

        #include <stdint.h>
#include <float.h>

float vectorized_s315(int iterations, int LEN_1D, float* a) {
    // First loop: fill array
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x, chksum;
    int index;

    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // Find max value using unrolled reduction
        float max_val = a[0];

        // Process 8 elements at a time
        float mv0 = a[0], mv1 = -FLT_MAX, mv2 = -FLT_MAX, mv3 = -FLT_MAX;
        float mv4 = -FLT_MAX, mv5 = -FLT_MAX, mv6 = -FLT_MAX, mv7 = -FLT_MAX;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);
        for (; i < limit; i += 8) {
            float v0 = a[i+0], v1 = a[i+1], v2 = a[i+2], v3 = a[i+3];
            float v4 = a[i+4], v5 = a[i+5], v6 = a[i+6], v7 = a[i+7];
            mv0 = v0 > mv0 ? v0 : mv0;
            mv1 = v1 > mv1 ? v1 : mv1;
            mv2 = v2 > mv2 ? v2 : mv2;
            mv3 = v3 > mv3 ? v3 : mv3;
            mv4 = v4 > mv4 ? v4 : mv4;
            mv5 = v5 > mv5 ? v5 : mv5;
            mv6 = v6 > mv6 ? v6 : mv6;
            mv7 = v7 > mv7 ? v7 : mv7;
        }
        // Combine partial maxima
        float mA = mv0 > mv1 ? mv0 : mv1;
        float mB = mv2 > mv3 ? mv2 : mv3;
        float mC = mv4 > mv5 ? mv4 : mv5;
        float mD = mv6 > mv7 ? mv6 : mv7;
        float mAB = mA > mB ? mA : mB;
        float mCD = mC > mD ? mC : mD;
        max_val = mAB > mCD ? mAB : mCD;

        // Scalar tail for max
        for (; i < LEN_1D; i++) {
            if (a[i] > max_val) max_val = a[i];
        }

        // Find first index with max value
        // Unrolled search for first occurrence
        int first_index = 0;
        int found = 0;

        for (int j = 0; j < LEN_1D && !found; j += 8) {
            int end = j + 8;
            if (end > LEN_1D) end = LEN_1D;
            for (int k = j; k < end; k++) {
                if (a[k] == max_val && first_index == 0) {
                    first_index = k;
                    found = 1;
                    break;
                }
            }
        }

        x = max_val;
        index = first_index;
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s315(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s315(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
