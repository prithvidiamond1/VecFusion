#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int nl_iterations = 10 * (iterations / 256);

    for (int nl = 0; nl < nl_iterations; nl++) {
        // Store initial values of aa[0][i] before any modifications
        float init_aa0[256];
        for (int i = 0; i < 256; i++) {
            init_aa0[i] = aa[0][i];
        }

        // Process each column independently
        for (int i = 0; i < 256; i++) {
            if (init_aa0[i] > 0.0f) {
                float prev = init_aa0[i];
                // Process with explicit dependency chain
                for (int j = 1; j < 256; j++) {
                    prev = prev + bb[j][i] * cc[j][i];
                    aa[j][i] = prev;
                }
            }
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int nl_iterations = 10 * (iterations / 256);

    for (int nl = 0; nl < nl_iterations; nl++) {
        // Store initial values of aa[0][i] before any modifications
        float init_aa0[256];
        for (int i = 0; i < 256; i++) {
            init_aa0[i] = aa[0][i];
        }

        // Process each column independently with vectorization
        for (int i = 0; i < 256; i++) {
            if (init_aa0[i] > 0.0f) {
                float prev = init_aa0[i];

                // Vectorized inner loop with explicit dependency chain
                // Process 4 elements at a time with scalar accumulation
                int j = 1;
                for (; j + 3 < 256; j += 4) {
                    // Load 4 bb and cc values
                    float4 bb_vec = {bb[j][i], bb[j+1][i], bb[j+2][i], bb[j+3][i]};
                    float4 cc_vec = {cc[j][i], cc[j+1][i], cc[j+2][i], cc[j+3][i]};

                    // Compute 4 products
                    float4 products = bb_vec * cc_vec;

                    // Scalar accumulation chain for dependency
                    prev = prev + products.x;
                    aa[j][i] = prev;

                    prev = prev + products.y;
                    aa[j+1][i] = prev;

                    prev = prev + products.z;
                    aa[j+2][i] = prev;

                    prev = prev + products.w;
                    aa[j+3][i] = prev;
                }

                // Scalar tail cleanup
                for (; j < 256; j++) {
                    prev = prev + bb[j][i] * cc[j][i];
                    aa[j][i] = prev;
                }
            }
        }
    }
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128]; float cc_scalar[128]; float cc_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar)); fill_f32(cc_scalar, arr_len, &seed); memcpy(cc_vector, cc_scalar, sizeof(cc_scalar));
                s275(iterations, aa_scalar, bb_scalar, cc_scalar); vectorized_s275(iterations, aa_vector, bb_vector, cc_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((cc_scalar[i]) - (cc_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter cc on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
