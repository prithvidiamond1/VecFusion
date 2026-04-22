#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s3110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;

    for (int nl = 0; nl < 100 * (iterations / 256); nl++) {
        // Initialize with first element
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        // First pass: find global maximum value (vectorizable reduction)
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                float val = aa[i][j];
                int cond = val > max;
                max = cond ? val : max;
            }
        }

        // Second pass: find first occurrence of max in row-major order
        // This can be vectorized with early exit optimization
        int found = 0;
        for (int i = 0; i < 256 && !found; i++) {
            for (int j = 0; j < 256 && !found; j++) {
                int is_max = (aa[i][j] == max);
                found = is_max;
                xindex = is_max ? i : xindex;
                yindex = is_max ? j : yindex;
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

    for (int nl = 0; nl < 100 * (iterations / 256); nl++) {
        // Initialize with first element
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        // Vectorized maximum reduction
        for (int i = 0; i < 256; i++) {
            // Process 8 elements at a time
            for (int j = 0; j < 256; j += 8) {
                // Load 8 consecutive floats
                float val0 = aa[i][j];
                float val1 = aa[i][j+1];
                float val2 = aa[i][j+2];
                float val3 = aa[i][j+3];
                float val4 = aa[i][j+4];
                float val5 = aa[i][j+5];
                float val6 = aa[i][j+6];
                float val7 = aa[i][j+7];

                // Compare and update max
                if (val0 > max) max = val0;
                if (val1 > max) max = val1;
                if (val2 > max) max = val2;
                if (val3 > max) max = val3;
                if (val4 > max) max = val4;
                if (val5 > max) max = val5;
                if (val6 > max) max = val6;
                if (val7 > max) max = val7;
            }
        }

        // Vectorized search for first occurrence of max
        int found = 0;
        for (int i = 0; i < 256 && !found; i++) {
            for (int j = 0; j < 256 && !found; j += 8) {
                // Check 8 elements in parallel
                int found0 = (aa[i][j] == max) && !found;
                int found1 = (aa[i][j+1] == max) && !found;
                int found2 = (aa[i][j+2] == max) && !found;
                int found3 = (aa[i][j+3] == max) && !found;
                int found4 = (aa[i][j+4] == max) && !found;
                int found5 = (aa[i][j+5] == max) && !found;
                int found6 = (aa[i][j+6] == max) && !found;
                int found7 = (aa[i][j+7] == max) && !found;

                // Update indices for first found element
                if (found0) { xindex = i; yindex = j; found = 1; }
                if (found1 && !found) { xindex = i; yindex = j+1; found = 1; }
                if (found2 && !found) { xindex = i; yindex = j+2; found = 1; }
                if (found3 && !found) { xindex = i; yindex = j+3; found = 1; }
                if (found4 && !found) { xindex = i; yindex = j+4; found = 1; }
                if (found5 && !found) { xindex = i; yindex = j+5; found = 1; }
                if (found6 && !found) { xindex = i; yindex = j+6; found = 1; }
                if (found7 && !found) { xindex = i; yindex = j+7; found = 1; }
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
