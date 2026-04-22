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

        // Process each row independently to find row maxima and their first occurrence
        float row_max[256];
        int row_xindex[256];
        int row_yindex[256];

        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_yindex = 0;

            // Vectorizable max-finding within row
            for (int j = 1; j < 256; j++) {
                float val = aa[i][j];
                if (val > local_max) {
                    local_max = val;
                    local_yindex = j;
                }
            }

            row_max[i] = local_max;
            row_xindex[i] = i;
            row_yindex[i] = local_yindex;
        }

        // Reduce across rows, preserving first occurrence in row-major order
        float global_max = row_max[0];
        int global_xindex = row_xindex[0];
        int global_yindex = row_yindex[0];

        for (int i = 1; i < 256; i++) {
            if (row_max[i] > global_max) {
                global_max = row_max[i];
                global_xindex = row_xindex[i];
                global_yindex = row_yindex[i];
            }
        }

        max = global_max;
        xindex = global_xindex;
        yindex = global_yindex;
        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}

        #include <float.h>

float vectorized_s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;

    for (int nl = 0; nl < 100 * (iterations / 256); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        float row_max[256];
        int row_xindex[256];
        int row_yindex[256];

        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_yindex = 0;

            // Unrolled/vector-friendly max-finding within row
            // Process 8 elements at a time
            float vm0, vm1, vm2, vm3, vm4, vm5, vm6, vm7;
            int vi0, vi1, vi2, vi3, vi4, vi5, vi6, vi7;

            vm0 = aa[i][0]; vi0 = 0;
            vm1 = aa[i][1]; vi1 = 1;
            vm2 = aa[i][2]; vi2 = 2;
            vm3 = aa[i][3]; vi3 = 3;
            vm4 = aa[i][4]; vi4 = 4;
            vm5 = aa[i][5]; vi5 = 5;
            vm6 = aa[i][6]; vi6 = 6;
            vm7 = aa[i][7]; vi7 = 7;

            for (int j = 8; j < 256; j += 8) {
                float v0 = aa[i][j+0];
                float v1 = aa[i][j+1];
                float v2 = aa[i][j+2];
                float v3 = aa[i][j+3];
                float v4 = aa[i][j+4];
                float v5 = aa[i][j+5];
                float v6 = aa[i][j+6];
                float v7 = aa[i][j+7];

                if (v0 > vm0) { vm0 = v0; vi0 = j+0; }
                if (v1 > vm1) { vm1 = v1; vi1 = j+1; }
                if (v2 > vm2) { vm2 = v2; vi2 = j+2; }
                if (v3 > vm3) { vm3 = v3; vi3 = j+3; }
                if (v4 > vm4) { vm4 = v4; vi4 = j+4; }
                if (v5 > vm5) { vm5 = v5; vi5 = j+5; }
                if (v6 > vm6) { vm6 = v6; vi6 = j+6; }
                if (v7 > vm7) { vm7 = v7; vi7 = j+7; }
            }

            // Reduce 8 lanes - preserve first occurrence (lower index wins on tie)
            // We want the first (lowest index) occurrence of the maximum
            // Compare pairwise, keeping lower index on tie
            if (vm1 > vm0) { vm0 = vm1; vi0 = vi1; }
            if (vm3 > vm2) { vm2 = vm3; vi2 = vi3; }
            if (vm5 > vm4) { vm4 = vm5; vi4 = vi5; }
            if (vm7 > vm6) { vm6 = vm7; vi6 = vi7; }

            if (vm2 > vm0) { vm0 = vm2; vi0 = vi2; }
            if (vm6 > vm4) { vm4 = vm6; vi4 = vi6; }

            if (vm4 > vm0) { vm0 = vm4; vi0 = vi4; }

            local_max = vm0;
            local_yindex = vi0;

            row_max[i] = local_max;
            row_xindex[i] = i;
            row_yindex[i] = local_yindex;
        }

        // Reduce across rows
        float global_max = row_max[0];
        int global_xindex = row_xindex[0];
        int global_yindex = row_yindex[0];

        for (int i = 1; i < 256; i++) {
            if (row_max[i] > global_max) {
                global_max = row_max[i];
                global_xindex = row_xindex[i];
                global_yindex = row_yindex[i];
            }
        }

        max = global_max;
        xindex = global_xindex;
        yindex = global_yindex;
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
