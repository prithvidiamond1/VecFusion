#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s342(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute mask (vectorizable)
        char mask[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (a[i] > (float)0.);
        }

        // Second pass: compute prefix sum of mask (j values)
        int indices[LEN_1D];
        int j = -1;
        for (int i = 0; i < LEN_1D; i++) {
            j += mask[i];
            indices[i] = j;
        }

        // Third pass: apply assignments using precomputed indices
        // Remove conditional branch to enable vectorization
        for (int i = 0; i < LEN_1D; i++) {
            // Use mask as selector: if mask[i] is 1, use b[indices[i]], else keep a[i]
            int idx = indices[i];
            float new_val = b[idx];
            float old_val = a[i];
            // Branchless selection using mask
            a[i] = mask[i] ? new_val : old_val;
        }
    }
}

        #include <string.h>

void vectorized_s342(int iterations, int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute mask (vectorizable)
        char mask[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (a[i] > (float)0.);
        }

        // Second pass: compute prefix sum of mask (j values)
        // This is inherently sequential due to the prefix sum dependency
        int indices[LEN_1D];
        int j = -1;
        for (int i = 0; i < LEN_1D; i++) {
            j += mask[i];
            indices[i] = j;
        }

        // Third pass: apply assignments using precomputed indices
        // Vectorizable: no loop-carried dependencies
        int i = 0;
        // Process 8 elements at a time
        for (; i <= LEN_1D - 8; i += 8) {
            int idx0 = indices[i+0];
            int idx1 = indices[i+1];
            int idx2 = indices[i+2];
            int idx3 = indices[i+3];
            int idx4 = indices[i+4];
            int idx5 = indices[i+5];
            int idx6 = indices[i+6];
            int idx7 = indices[i+7];

            float new_val0 = b[idx0];
            float new_val1 = b[idx1];
            float new_val2 = b[idx2];
            float new_val3 = b[idx3];
            float new_val4 = b[idx4];
            float new_val5 = b[idx5];
            float new_val6 = b[idx6];
            float new_val7 = b[idx7];

            float old_val0 = a[i+0];
            float old_val1 = a[i+1];
            float old_val2 = a[i+2];
            float old_val3 = a[i+3];
            float old_val4 = a[i+4];
            float old_val5 = a[i+5];
            float old_val6 = a[i+6];
            float old_val7 = a[i+7];

            a[i+0] = mask[i+0] ? new_val0 : old_val0;
            a[i+1] = mask[i+1] ? new_val1 : old_val1;
            a[i+2] = mask[i+2] ? new_val2 : old_val2;
            a[i+3] = mask[i+3] ? new_val3 : old_val3;
            a[i+4] = mask[i+4] ? new_val4 : old_val4;
            a[i+5] = mask[i+5] ? new_val5 : old_val5;
            a[i+6] = mask[i+6] ? new_val6 : old_val6;
            a[i+7] = mask[i+7] ? new_val7 : old_val7;
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            int idx = indices[i];
            float new_val = b[idx];
            float old_val = a[i];
            a[i] = mask[i] ? new_val : old_val;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s342(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s342(iterations, LEN_1D, a_vector, b_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
