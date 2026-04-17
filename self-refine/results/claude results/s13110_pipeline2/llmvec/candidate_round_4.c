#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s13110(int iterations, float aa[256][256])
{
    int xindex, yindex;
    float max, chksum;
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        float local_max[256];
        int local_yindex[256];

        // Initialize local arrays
        for (int i = 0; i < 256; i++) {
            local_max[i] = aa[i][0];
            local_yindex[i] = 0;
        }

        // Inner loop: find max value per row (pure value reduction, vectorizable)
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                int cond = aa[i][j] > local_max[i];
                local_max[i] = cond * aa[i][j] + (1 - cond) * local_max[i];
            }
        }

        // Find max index per row separately (split from value reduction)
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                int cond = aa[i][j] == local_max[i];
                local_yindex[i] = cond * j + (1 - cond) * local_yindex[i];
            }
        }

        // Find global max value (vectorizable reduction)
        float global_max = local_max[0];
        for (int i = 1; i < 256; i++) {
            global_max = local_max[i] > global_max ? local_max[i] : global_max;
        }

        // Find xindex: first scan for matching row (split into single variable update)
        for (int i = 0; i < 256; i++) {
            int cond = local_max[i] == global_max;
            xindex = cond * i + (1 - cond) * xindex;
        }

        // Find yindex from xindex directly (no loop needed)
        yindex = local_yindex[xindex];

        max = global_max;

        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}

        ```c
#include <stdint.h>
#include <string.h>

float vectorized_s13110(int iterations, float aa[256][256])
{
    int xindex, yindex;
    float max, chksum;
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        float local_max[256];
        int local_yindex[256];

        // Initialize local arrays
        for (int i = 0; i < 256; i++) {
            local_max[i] = aa[i][0];
            local_yindex[i] = 0;
        }

        // Inner loop: find max value per row, unrolled by 8
        for (int i = 0; i < 256; i++) {
            float m0 = local_max[i];
            float m1 = m0, m2 = m0, m3 = m0;
            float m4 = m0, m5 = m0, m6 = m0, m7 = m0;
            int j = 1;
            for (; j + 7 < 256; j += 8) {
                float v0 = aa[i][j+0]; m0 = v0 > m0 ? v0 : m0;
                float v1 = aa[i][j+1]; m1 = v1 > m1 ? v1 : m1;
                float v2 = aa[i][j+2]; m2 = v2 > m2 ? v2 : m2;
                float v3 = aa[i][j+3]; m3 = v3 > m3 ? v3 : m3;
                float v4 = aa[i][j+4]; m4 = v4 > m4 ? v4 : m4;
                float v5 = aa[i][j+5]; m5 = v5 > m5 ? v5 : m5;
                float v6 = aa[i][j+6]; m6 = v6 > m6 ? v6 : m6;
                float v7 = aa[i][j+7]; m7 = v7 > m7 ? v7 : m7;
            }
            // merge partial maxes
            float ma = m0 > m1 ? m0 : m1;
            float mb = m2 > m3 ? m2 : m3;
            float mc = m4 > m5 ? m4 : m5;
            float md = m6 > m7 ? m6 : m7;
            float me = ma > mb ? ma : mb;
            float mf = mc > md ? mc : md;
            float mg = me > mf ? me : mf;
            for (; j < 256; j++) {
                float v = aa[i][j]; mg = v > mg ? v : mg;
            }
            local_max[i] = mg;
        }

        // Find max index per row separately
        for (int i = 0; i < 256; i++) {
            int yi = 0;
            float lm = local_max[i];
            for (int j = 1; j < 256; j++) {
                int cond = aa[i][j] == lm;
                yi = cond * j + (1 - cond) * yi;
            }
            local_yindex[i] = yi;
        }

        // Find global max value with unrolling
        float gm0 = local_max[0];
        float gm1 = gm0, gm2 = gm0, gm3 = gm0;
        float gm4 = gm0, gm5 = gm0, gm6 = gm0, gm7 = gm0;
        int i = 1;
        for (; i + 7 < 256; i += 8) {
            float v0 = local_max[i+0]; gm0 = v0 > gm0 ? v0 : gm0;
            float v1 = local_max[i+1]; gm1 = v1 > gm1 ? v1 : gm1;
            float v2 = local_max[i+2]; gm2 = v2 > gm2 ? v2 : gm2;
            float v3 = local_max[i+3]; gm3 = v3 > gm3 ? v3 : gm3;
            float v4 = local_max[i+4]; gm4 = v4 > gm4 ? v4 : gm4;
            float v5 = local_max[i+5]; gm5 = v5 > gm5 ? v5 : gm5;
            float v6 = local_max[i+6]; gm6 = v6 > gm6 ? v6 : gm6;
            float v7 = local_max[i+7]; gm7 = v7 > gm7 ? v7 : gm7;
        }
        float ga = gm0 > gm1 ? gm0 : gm1;
        float gb = gm2 > gm3 ? gm2 : gm3;
        float gc = gm4 > gm5 ? gm4 : gm5;
        float gd = gm6 > gm7 ? gm6 : gm7;
        float ge = ga > gb ? ga : gb;
        float gf = gc > gd ? gc : gd;
        float global_max = ge > gf ? ge : gf;
        for (; i < 256; i++) {
            float v = local_max[i]; global_max = v > global_max ? v : global_max;
        }

        // Find xindex
        for (int k = 0; k < 256; k++) {
            int cond = local_max[k] == global_max;
            xindex = cond * k + (1 - cond) * xindex;
        }

        yindex = local_yindex[xindex];
        max = global_max;
        chksum = max + (float)xindex + (float)yindex;
    }
    return

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
