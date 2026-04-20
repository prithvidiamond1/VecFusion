#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s3110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        // First pass: find max value only (fully vectorizable)
        float row_max_arr[256];
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            for (int j = 1; j < 256; j++) {
                float val = aa[i][j];
                row_max = (val > row_max) ? val : row_max;
            }
            row_max_arr[i] = row_max;
        }

        // Reduce across rows to find global max
        for (int i = 0; i < 256; i++) {
            if (row_max_arr[i] > max) {
                max = row_max_arr[i];
            }
        }

        // Second pass: find last occurrence of max (vectorizable per row)
        // Process rows in reverse order to match original "last occurrence" semantics
        int found = 0;
        for (int i = 255; i >= 0 && !found; i--) {
            if (row_max_arr[i] == max) {
                // Scan this row from end to beginning for last occurrence
                for (int j = 255; j >= 0; j--) {
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

    return max + xindex + 1 + yindex + 1;
}

        #include <stdint.h>
#include <string.h>

float vectorized_s3110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        // Vectorized row max computation
        float row_max_arr[256];
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            // Process 4 elements at a time
            int j;
            for (j = 1; j <= 252; j += 4) {
                float val0 = aa[i][j];
                float val1 = aa[i][j+1];
                float val2 = aa[i][j+2];
                float val3 = aa[i][j+3];

                float max0 = (val0 > row_max) ? val0 : row_max;
                float max1 = (val1 > max0) ? val1 : max0;
                float max2 = (val2 > max1) ? val2 : max1;
                row_max = (val3 > max2) ? val3 : max2;
            }
            // Handle remaining elements
            for (; j < 256; j++) {
                float val = aa[i][j];
                row_max = (val > row_max) ? val : row_max;
            }
            row_max_arr[i] = row_max;
        }

        // Vectorized reduction across rows (4 at a time)
        float max_arr[4] = {max, max, max, max};
        int i;
        for (i = 0; i <= 252; i += 4) {
            float r0 = row_max_arr[i];
            float r1 = row_max_arr[i+1];
            float r2 = row_max_arr[i+2];
            float r3 = row_max_arr[i+3];

            max_arr[0] = (r0 > max_arr[0]) ? r0 : max_arr[0];
            max_arr[1] = (r1 > max_arr[1]) ? r1 : max_arr[1];
            max_arr[2] = (r2 > max_arr[2]) ? r2 : max_arr[2];
            max_arr[3] = (r3 > max_arr[3]) ? r3 : max_arr[3];
        }
        // Final reduction of 4 partial maxes
        max = max_arr[0];
        for (int k = 1; k < 4; k++) {
            if (max_arr[k] > max) max = max_arr[k];
        }
        // Handle remaining rows
        for (; i < 256; i++) {
            if (row_max_arr[i] > max) {
                max = row_max_arr[i];
            }
        }

        // Find last occurrence of max
        int found = 0;
        for (int i = 255; i >= 0 && !found; i--) {
            if (row_max_arr[i] == max) {
                // Vectorized row scan (4 at a time, reverse order)
                int j;
                for (j = 255; j >= 3 && !found; j -= 4) {
                    float val0 = aa[i][j];
                    float val1 = aa[i][j-1];
                    float val2 = aa[i][j-2];
                    float val3 = aa[i][j-3];

                    if (val0 == max) {
                        xindex = i;
                        yindex = j;
                        found = 1;
                    } else if (val1 == max) {
                        xindex = i;
                        yindex = j-1;
                        found = 1;
                    } else if (val2 == max) {
                        xindex = i;
                        yindex = j-2;
                        found = 1;
                    } else if (val3 == max) {
                        xindex = i;
                        yindex = j-3;
                        found = 1;
                    }
                }
                // Handle remaining elements
                for (; j >= 0 && !found; j--) {
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

    return max + xindex + 1 + yindex + 1;
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
                ret_scalar = s3110(iterations, aa_scalar); ret_vector = vectorized_s3110(iterations, aa_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
