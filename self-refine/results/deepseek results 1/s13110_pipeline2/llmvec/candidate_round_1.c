#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        // Process each row independently
        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_yidx = 0;
            float* row_ptr = aa[i];

            // Single pass through row with direct comparison
            for (int j = 1; j < 256; j++) {
                float current = row_ptr[j];
                if (current > local_max) {
                    local_max = current;
                    local_yidx = j;
                }
            }

            // Compare row maximum with global maximum
            if (local_max > max) {
                max = local_max;
                xindex = i;
                yindex = local_yidx;
            } else if (local_max == max) {
                // Keep original semantics: only update if strictly greater
                // No update needed when equal
            }
        }

        chksum = max + (float)xindex + (float)yindex;
    }
    return max + (float)(xindex + 1) + (float)(yindex + 1);
}

        #include <float.h>

float vectorized_s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_yidx = 0;
            float* row_ptr = aa[i];

            // Unroll by 8 for vectorization-friendly code
            // Process chunks of 8
            int j = 1;
            // We'll track 8 lanes of max values and indices
            float vmax0 = -FLT_MAX, vmax1 = -FLT_MAX, vmax2 = -FLT_MAX, vmax3 = -FLT_MAX;
            float vmax4 = -FLT_MAX, vmax5 = -FLT_MAX, vmax6 = -FLT_MAX, vmax7 = -FLT_MAX;
            int vidx0 = 1, vidx1 = 2, vidx2 = 3, vidx3 = 4;
            int vidx4 = 5, vidx5 = 6, vidx6 = 7, vidx7 = 8;

            for (; j + 7 < 256; j += 8) {
                float c0 = row_ptr[j+0];
                float c1 = row_ptr[j+1];
                float c2 = row_ptr[j+2];
                float c3 = row_ptr[j+3];
                float c4 = row_ptr[j+4];
                float c5 = row_ptr[j+5];
                float c6 = row_ptr[j+6];
                float c7 = row_ptr[j+7];

                if (c0 > vmax0) { vmax0 = c0; vidx0 = j+0; }
                if (c1 > vmax1) { vmax1 = c1; vidx1 = j+1; }
                if (c2 > vmax2) { vmax2 = c2; vidx2 = j+2; }
                if (c3 > vmax3) { vmax3 = c3; vidx3 = j+3; }
                if (c4 > vmax4) { vmax4 = c4; vidx4 = j+4; }
                if (c5 > vmax5) { vmax5 = c5; vidx5 = j+5; }
                if (c6 > vmax6) { vmax6 = c6; vidx6 = j+6; }
                if (c7 > vmax7) { vmax7 = c7; vidx7 = j+7; }
            }

            // Reduce 8 lanes
            if (vmax1 > vmax0) { vmax0 = vmax1; vidx0 = vidx1; }
            if (vmax3 > vmax2) { vmax2 = vmax3; vidx2 = vidx3; }
            if (vmax5 > vmax4) { vmax4 = vmax5; vidx4 = vidx5; }
            if (vmax7 > vmax6) { vmax6 = vmax7; vidx6 = vidx7; }
            if (vmax2 > vmax0) { vmax0 = vmax2; vidx0 = vidx2; }
            if (vmax6 > vmax4) { vmax4 = vmax6; vidx4 = vidx6; }
            if (vmax4 > vmax0) { vmax0 = vmax4; vidx0 = vidx4; }

            // Compare with local_max (which is aa[i][0])
            if (vmax0 > local_max) {
                local_max = vmax0;
                local_yidx = vidx0;
            }

            // Scalar cleanup tail
            for (; j < 256; j++) {
                float current = row_ptr[j];
                if (current > local_max) {
                    local_max = current;
                    local_yidx = j;
                }
            }

            if (local_max > max) {
                max = local_max;
                xindex = i;
                yindex = local_yidx;
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
