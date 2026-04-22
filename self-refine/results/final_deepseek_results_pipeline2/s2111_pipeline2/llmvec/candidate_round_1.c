#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2111(int iterations, float aa[256][256]) {
    int loop_bound = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_bound; nl++) {
        // Process first row (j=1) separately
        float prev_val = aa[1][0];
        for (int i = 1; i < 256; i++) {
            float new_val = (prev_val + aa[0][i]) / 1.9f;
            aa[1][i] = new_val;
            prev_val = new_val;
        }

        // Process remaining rows (j=2 to 255)
        for (int j = 2; j < 256; j++) {
            // Copy previous row to break vertical dependency
            float prev_row[256];
            for (int i = 0; i < 256; i++) {
                prev_row[i] = aa[j-1][i];
            }

            // Process current row with only horizontal dependency
            float current_val = (aa[j][0] + prev_row[1]) / 1.9f;
            aa[j][1] = current_val;

            for (int i = 2; i < 256; i++) {
                current_val = (current_val + prev_row[i]) / 1.9f;
                aa[j][i] = current_val;
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s2111(int iterations, float aa[256][256]) {
    int loop_bound = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_bound; nl++) {
        // Process first row (j=1) separately
        float prev_val = aa[1][0];
        for (int i = 1; i < 256; i++) {
            float new_val = (prev_val + aa[0][i]) / 1.9f;
            aa[1][i] = new_val;
            prev_val = new_val;
        }

        // Process remaining rows (j=2 to 255)
        for (int j = 2; j < 256; j++) {
            // Copy previous row to break vertical dependency
            float prev_row[256];
            for (int i = 0; i < 256; i++) {
                prev_row[i] = aa[j-1][i];
            }

            // Process current row with only horizontal dependency
            float current_val = (aa[j][0] + prev_row[1]) / 1.9f;
            aa[j][1] = current_val;

            // Vectorized inner loop
            int i;
            for (i = 2; i <= 253; i += 4) {
                // Process 4 elements at a time
                float vals[4];
                vals[0] = (current_val + prev_row[i]) / 1.9f;
                current_val = vals[0];
                aa[j][i] = vals[0];

                vals[1] = (current_val + prev_row[i+1]) / 1.9f;
                current_val = vals[1];
                aa[j][i+1] = vals[1];

                vals[2] = (current_val + prev_row[i+2]) / 1.9f;
                current_val = vals[2];
                aa[j][i+2] = vals[2];

                vals[3] = (current_val + prev_row[i+3]) / 1.9f;
                current_val = vals[3];
                aa[j][i+3] = vals[3];
            }

            // Scalar cleanup for remaining elements
            for (; i < 256; i++) {
                current_val = (current_val + prev_row[i]) / 1.9f;
                aa[j][i] = current_val;
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
