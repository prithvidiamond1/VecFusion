#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Step 1: Find per-row maxima and their first column indices
        float row_max[256];
        int row_max_col[256];

        for (int i = 0; i < 256; i++) {
            float rmax = aa[i][0];
            int rcol = 0;

            // Vectorizable: find first occurrence of max in each row
            for (int j = 1; j < 256; j++) {
                float current = aa[i][j];
                // Use > to get first occurrence (matches original)
                int is_greater = current > rmax;
                rmax = is_greater ? current : rmax;
                rcol = is_greater ? j : rcol;
            }

            row_max[i] = rmax;
            row_max_col[i] = rcol;
        }

        // Step 2: Find global maximum across rows and first row index
        max = row_max[0];
        int max_row = 0;

        // Vectorizable reduction for max value and first row index
        for (int i = 1; i < 256; i++) {
            float current = row_max[i];
            int is_greater = current > max;
            max = is_greater ? current : max;
            max_row = is_greater ? i : max_row;
        }

        // Step 3: For rows with same max value, find first occurrence in row-major order
        for (int i = 0; i < max_row; i++) {
            if (row_max[i] == max) {
                // Compare column indices to maintain first occurrence in row-major order
                if (row_max_col[i] < row_max_col[max_row]) {
                    max_row = i;
                }
            }
        }

        // Step 4: Get column index from pre-computed row_max_col
        xindex = max_row;
        yindex = row_max_col[max_row];

        chksum = max + (float)xindex + (float)yindex;
    }

    return max + (float)(xindex + 1) + (float)(yindex + 1);
}

        #include <stdint.h>
#include <string.h>

float vectorized_s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Step 1: Find per-row maxima and their first column indices
        float row_max[256];
        int row_max_col[256];

        for (int i = 0; i < 256; i++) {
            float rmax = aa[i][0];
            int rcol = 0;

            // Vectorized inner loop
            for (int j = 1; j < 256; j += 4) {
                // Process 4 elements at a time
                for (int k = 0; k < 4 && (j + k) < 256; k++) {
                    float current = aa[i][j + k];
                    int is_greater = current > rmax;
                    rmax = is_greater ? current : rmax;
                    rcol = is_greater ? (j + k) : rcol;
                }
            }

            row_max[i] = rmax;
            row_max_col[i] = rcol;
        }

        // Step 2: Find global maximum across rows and first row index
        max = row_max[0];
        int max_row = 0;

        // Vectorized reduction for max value and first row index
        for (int i = 1; i < 256; i += 4) {
            // Process 4 rows at a time
            for (int k = 0; k < 4 && (i + k) < 256; k++) {
                float current = row_max[i + k];
                int is_greater = current > max;
                max = is_greater ? current : max;
                max_row = is_greater ? (i + k) : max_row;
            }
        }

        // Step 3: For rows with same max value, find first occurrence in row-major order
        for (int i = 0; i < max_row; i++) {
            if (row_max[i] == max) {
                // Compare column indices to maintain first occurrence in row-major order
                if (row_max_col[i] < row_max_col[max_row]) {
                    max_row = i;
                }
            }
        }

        // Step 4: Get column index from pre-computed row_max_col
        xindex = max_row;
        yindex = row_max_col[max_row];

        chksum = max + (float)xindex + (float)yindex;
    }

    return max + (float)(xindex + 1) + (float)(yindex + 1);
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                ret_scalar = s13110(iterations, aa_scalar); ret_vector = vectorized_s13110(iterations, aa_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
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
