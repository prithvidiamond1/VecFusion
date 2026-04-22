#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2111(int iterations, float aa[256][256]) {
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process first row (j=1) separately
        float left_val = aa[1][0];
        for (int i = 1; i < 256; i++) {
            float new_val = (left_val + aa[0][i]) / 1.9f;
            left_val = new_val;
            aa[1][i] = new_val;
        }

        // Process remaining rows (j=2 to 255)
        for (int j = 2; j < 256; j++) {
            // Copy previous row to temporary array
            float prev_row[256];
            for (int k = 0; k < 256; k++) {
                prev_row[k] = aa[j-1][k];
            }

            // Process current row
            float left = aa[j][0];
            for (int i = 1; i < 256; i++) {
                float new_val = (left + prev_row[i]) / 1.9f;
                left = new_val;
                aa[j][i] = new_val;
            }
        }
    }
}

        #include <string.h>

void vectorized_s2111(int iterations, float aa[256][256]) {
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process first row (j=1) separately
        // This loop has a carried dependency (left_val depends on previous iteration)
        // so we cannot vectorize it directly; keep scalar
        float left_val = aa[1][0];
        for (int i = 1; i < 256; i++) {
            float new_val = (left_val + aa[0][i]) / 1.9f;
            left_val = new_val;
            aa[1][i] = new_val;
        }

        // Process remaining rows (j=2 to 255)
        for (int j = 2; j < 256; j++) {
            // Copy previous row to temporary array using memcpy (vectorizable)
            float prev_row[256];
            memcpy(prev_row, aa[j-1], 256 * sizeof(float));

            // Process current row - scalar due to carried dependency on left
            float left = aa[j][0];
            for (int i = 1; i < 256; i++) {
                float new_val = (left + prev_row[i]) / 1.9f;
                left = new_val;
                aa[j][i] = new_val;
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                s2111(iterations, aa_scalar); vectorized_s2111(iterations, aa_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
