#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;

    for (int nl = 0; nl < 100 * (iterations / 256); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        float row_max[256];
        int row_yindex[256];

        // First: compute row maxima and their column indices
        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_y = 0;

            // Process all columns with branchless updates
            for (int j = 1; j < 256; j++) {
                float val = aa[i][j];
                int cond = val > local_max;
                local_max = cond ? val : local_max;
                local_y = cond ? j : local_y;
            }
            row_max[i] = local_max;
            row_yindex[i] = local_y;
        }

        // Second: find global maximum across rows
        for (int i = 0; i < 256; i++) {
            float val = row_max[i];
            int cond = val > max;
            max = cond ? val : max;
            xindex = cond ? i : xindex;
            yindex = cond ? row_yindex[i] : yindex;
        }

        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}

        #include <stdint.h>
#include <string.h>

float vectorized_s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;

    int outer_loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_loop_count; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        float row_max[256];
        int row_yindex[256];

        // Vectorized row maxima computation
        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_y = 0;

            // Process columns in chunks of 4 for vectorization
            int j;
            for (j = 1; j <= 256 - 4; j += 4) {
                float val0 = aa[i][j];
                float val1 = aa[i][j + 1];
                float val2 = aa[i][j + 2];
                float val3 = aa[i][j + 3];

                // Compare and update for each element
                if (val0 > local_max) {
                    local_max = val0;
                    local_y = j;
                }
                if (val1 > local_max) {
                    local_max = val1;
                    local_y = j + 1;
                }
                if (val2 > local_max) {
                    local_max = val2;
                    local_y = j + 2;
                }
                if (val3 > local_max) {
                    local_max = val3;
                    local_y = j + 3;
                }
            }

            // Process remaining columns
            for (; j < 256; j++) {
                float val = aa[i][j];
                if (val > local_max) {
                    local_max = val;
                    local_y = j;
                }
            }

            row_max[i] = local_max;
            row_yindex[i] = local_y;
        }

        // Vectorized global maximum search
        for (int i = 0; i <= 256 - 4; i += 4) {
            float val0 = row_max[i];
            float val1 = row_max[i + 1];
            float val2 = row_max[i + 2];
            float val3 = row_max[i + 3];

            if (val0 > max) {
                max = val0;
                xindex = i;
                yindex = row_yindex[i];
            }
            if (val1 > max) {
                max = val1;
                xindex = i + 1;
                yindex = row_yindex[i + 1];
            }
            if (val2 > max) {
                max = val2;
                xindex = i + 2;
                yindex = row_yindex[i + 2];
            }
            if (val3 > max) {
                max = val3;
                xindex = i + 3;
                yindex = row_yindex[i + 3];
            }
        }

        // Process remaining rows
        for (int i = (256 / 4) * 4; i < 256; i++) {
            float val = row_max[i];
            if (val > max) {
                max = val;
                xindex = i;
                yindex = row_yindex[i];
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
