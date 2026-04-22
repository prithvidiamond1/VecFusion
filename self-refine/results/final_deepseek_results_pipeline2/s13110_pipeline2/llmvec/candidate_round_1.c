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
        // First pass: find row-wise max values (vectorizable)
        float row_max_vals[256];
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            for (int j = 1; j < 256; j++) {
                float current = aa[i][j];
                if (current > row_max) {
                    row_max = current;
                }
            }
            row_max_vals[i] = row_max;
        }

        // Second pass: find global max value across rows (vectorizable)
        max = row_max_vals[0];
        for (int i = 1; i < 256; i++) {
            if (row_max_vals[i] > max) {
                max = row_max_vals[i];
            }
        }

        // Third pass: find first occurrence of global max (row-major order)
        xindex = 0;
        yindex = 0;
        int found = 0;
        for (int i = 0; i < 256 && !found; i++) {
            if (row_max_vals[i] == max) {
                for (int j = 0; j < 256 && !found; j++) {
                    if (aa[i][j] == max) {
                        xindex = i;
                        yindex = j;
                        found = 1;
                        break;
                    }
                }
            }
        }

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
        // First pass: find row-wise max values (vectorizable)
        float row_max_vals[256];
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            // Vector-friendly loop with explicit bounds
            int j = 1;
            // Process in chunks of 4 for better vectorization
            for (; j <= 252; j += 4) {
                float current0 = aa[i][j];
                float current1 = aa[i][j+1];
                float current2 = aa[i][j+2];
                float current3 = aa[i][j+3];

                if (current0 > row_max) row_max = current0;
                if (current1 > row_max) row_max = current1;
                if (current2 > row_max) row_max = current2;
                if (current3 > row_max) row_max = current3;
            }
            // Scalar cleanup for remaining elements
            for (; j < 256; j++) {
                float current = aa[i][j];
                if (current > row_max) {
                    row_max = current;
                }
            }
            row_max_vals[i] = row_max;
        }

        // Second pass: find global max value across rows (vectorizable)
        max = row_max_vals[0];
        // Process in chunks of 4
        int i = 1;
        for (; i <= 252; i += 4) {
            float val0 = row_max_vals[i];
            float val1 = row_max_vals[i+1];
            float val2 = row_max_vals[i+2];
            float val3 = row_max_vals[i+3];

            if (val0 > max) max = val0;
            if (val1 > max) max = val1;
            if (val2 > max) max = val2;
            if (val3 > max) max = val3;
        }
        // Scalar cleanup for remaining elements
        for (; i < 256; i++) {
            if (row_max_vals[i] > max) {
                max = row_max_vals[i];
            }
        }

        // Third pass: find first occurrence of global max (row-major order)
        // Convert to canonical counted loop with explicit bounds
        xindex = 0;
        yindex = 0;
        int found = 0;
        // Outer loop with explicit bounds
        for (int i = 0; i < 256; i++) {
            if (found) break;
            if (row_max_vals[i] == max) {
                // Inner loop with explicit bounds
                for (int j = 0; j < 256; j++) {
                    if (aa[i][j] == max) {
                        xindex = i;
                        yindex = j;
                        found = 1;
                        break;
                    }
                }
            }
        }

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
