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

        typedef float __attribute__((ext_vector_type(4))) float4;
typedef int __attribute__((ext_vector_type(4))) int4;

float vectorized_s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max;
    int nl_limit = 100 * (iterations / 256);
    for (int nl = 0; nl < nl_limit; nl++) {
        float4 max_vec = {aa[0][0], aa[0][0], aa[0][0], aa[0][0]};
        int xindex_vec[4] = {0, 0, 0, 0};
        int yindex_vec[4] = {0, 0, 0, 0};

        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j += 4) {
                float4 current = *(float4*)&aa[i][j];
                int4 mask = current > max_vec;

                // Update max where current > max
                max_vec = __builtin_shufflevector(max_vec, current, 
                    mask[0] ? 4 : 0,
                    mask[1] ? 5 : 1,
                    mask[2] ? 6 : 2,
                    mask[3] ? 7 : 3);

                // Update xindex and yindex where current > max
                if (mask[0]) {
                    xindex_vec[0] = i;
                    yindex_vec[0] = j;
                }
                if (mask[1]) {
                    xindex_vec[1] = i;
                    yindex_vec[1] = j + 1;
                }
                if (mask[2]) {
                    xindex_vec[2] = i;
                    yindex_vec[2] = j + 2;
                }
                if (mask[3]) {
                    xindex_vec[3] = i;
                    yindex_vec[3] = j + 3;
                }
            }
        }

        // Reduce across vector lanes
        float max_val = max_vec[0];
        int xindex_val = xindex_vec[0];
        int yindex_val = yindex_vec[0];
        for (int k = 1; k < 4; k++) {
            if (max_vec[k] > max_val) {
                max_val = max_vec[k];
                xindex_val = xindex_vec[k];
                yindex_val = yindex_vec[k];
            }
        }

        max = max_val;
        xindex = xindex_val;
        yindex = yindex_val;
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
