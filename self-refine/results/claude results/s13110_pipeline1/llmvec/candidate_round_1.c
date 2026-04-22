#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s13110(int iterations,float aa[256][256])
{
    int xindex, yindex;
    float max, chksum;
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        max = aa[(0)][0];
        xindex = 0;
        yindex = 0;
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                if (aa[i][j] > max) {
                    max = aa[i][j];
                    xindex = i;
                    yindex = j;
                }
            }
        }
        chksum = max + (float) xindex + (float) yindex;
    }
    return max + xindex+1 + yindex+1;
}

        #include <stdint.h>
#include <string.h>

float vectorized_s13110(int iterations, float aa[256][256])
{
    int xindex, yindex;
    float max, chksum;

    for (int nl = 0; nl < 100 * (iterations / (256)); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        for (int i = 0; i < 256; i++) {
            // Process 8 floats at a time using unrolled scalar (vector-friendly)
            // We track max and its index across j dimension
            float local_max0 = aa[i][0];   int local_j0 = 0;
            float local_max1 = aa[i][1];   int local_j1 = 1;
            float local_max2 = aa[i][2];   int local_j2 = 2;
            float local_max3 = aa[i][3];   int local_j3 = 3;
            float local_max4 = aa[i][4];   int local_j4 = 4;
            float local_max5 = aa[i][5];   int local_j5 = 5;
            float local_max6 = aa[i][6];   int local_j6 = 6;
            float local_max7 = aa[i][7];   int local_j7 = 7;

            for (int j = 8; j < 256; j += 8) {
                float v0 = aa[i][j+0];
                float v1 = aa[i][j+1];
                float v2 = aa[i][j+2];
                float v3 = aa[i][j+3];
                float v4 = aa[i][j+4];
                float v5 = aa[i][j+5];
                float v6 = aa[i][j+6];
                float v7 = aa[i][j+7];

                if (v0 > local_max0) { local_max0 = v0; local_j0 = j+0; }
                if (v1 > local_max1) { local_max1 = v1; local_j1 = j+1; }
                if (v2 > local_max2) { local_max2 = v2; local_j2 = j+2; }
                if (v3 > local_max3) { local_max3 = v3; local_j3 = j+3; }
                if (v4 > local_max4) { local_max4 = v4; local_j4 = j+4; }
                if (v5 > local_max5) { local_max5 = v5; local_j5 = j+5; }
                if (v6 > local_max6) { local_max6 = v6; local_j6 = j+6; }
                if (v7 > local_max7) { local_max7 = v7; local_j7 = j+7; }
            }

            // Reduce 8 lanes
            float row_max = local_max0; int row_j = local_j0;
            if (local_max1 > row_max) { row_max = local_max1; row_j = local_j1; }
            if (local_max2 > row_max) { row_max = local_max2; row_j = local_j2; }
            if (local_max3 > row_max) { row_max = local_max3; row_j = local_j3; }
            if (local_max4 > row_max) { row_max = local_max4; row_j = local_j4; }
            if (local_max5 > row_max) { row_max = local_max5; row_j = local_j5; }
            if (local_max6 > row_max) { row_max = local_max6; row_j = local_j6; }
            if (local_max7 > row_max) { row_max = local_max7; row_j = local_j7; }

            if (row_max > max) {
                max = row_max;
                xindex = i;
                yindex = row_j;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                ret_scalar = s13110(iterations, aa_scalar); ret_vector = vectorized_s13110(iterations, aa_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < n; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
