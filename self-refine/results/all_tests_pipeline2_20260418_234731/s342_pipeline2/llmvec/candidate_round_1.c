#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s342(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute prefix sum of conditions into a temporary array
        int prefix[LEN_1D];
        int count = -1;
        for (int i = 0; i < LEN_1D; i++) {
            int cond = (a[i] > (float)0.);
            count += cond;
            prefix[i] = cond ? count : -1;
        }

        // Second loop: vectorizable assignment using precomputed indices
        for (int i = 0; i < LEN_1D; i++) {
            int idx = prefix[i];
            if (idx != -1) {
                a[i] = b[idx];
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s342(int iterations, int LEN_1D, float* a, float *b) {
    for (int nl = 0; nl < iterations; nl++) {
        int prefix[LEN_1D];
        int count = -1;

        // First loop: compute prefix sum of conditions
        for (int i = 0; i < LEN_1D; i++) {
            int cond = (a[i] > 0.0f);
            count += cond;
            prefix[i] = cond ? count : -1;
        }

        // Second loop: vectorized assignment
        int i = 0;
        #if defined(__clang__)
        typedef float float4 __attribute__((ext_vector_type(4)));
        typedef int int4 __attribute__((ext_vector_type(4)));

        for (; i + 3 < LEN_1D; i += 4) {
            // Load indices
            int idx0 = prefix[i];
            int idx1 = prefix[i + 1];
            int idx2 = prefix[i + 2];
            int idx3 = prefix[i + 3];

            // Check which indices are valid
            int4 valid_mask = (int4){idx0 != -1, idx1 != -1, idx2 != -1, idx3 != -1};

            // Gather values from b using valid indices
            float4 gathered;
            gathered.x = (valid_mask.x) ? b[idx0] : a[i];
            gathered.y = (valid_mask.y) ? b[idx1] : a[i + 1];
            gathered.z = (valid_mask.z) ? b[idx2] : a[i + 2];
            gathered.w = (valid_mask.w) ? b[idx3] : a[i + 3];

            // Store back to a
            a[i] = gathered.x;
            a[i + 1] = gathered.y;
            a[i + 2] = gathered.z;
            a[i + 3] = gathered.w;
        }
        #endif

        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            int idx = prefix[i];
            if (idx != -1) {
                a[i] = b[idx];
            }
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
