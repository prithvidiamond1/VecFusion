#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256]) {
    for (int nl = 0; nl < 10 * (iterations / 256); nl++) {
        // Create mask for columns where aa[0][i] > 0
        int mask[256];
        for (int i = 0; i < 256; i++) {
            mask[i] = (aa[0][i] > (float)0.) ? 1 : 0;
        }

        // Process columns with mask-based computation
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                if (mask[i]) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256]) {
    typedef float v8f __attribute__((__vector_size__(32), __aligned__(32)));
    typedef int v8i __attribute__((__vector_size__(32), __aligned__(32)));

    const int outer_loop_count = 10 * (iterations / 256);

    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Vectorized mask creation
        v8i mask_vec[32];
        for (int vi = 0; vi < 32; vi++) {
            int base = vi * 8;
            v8f a0_vec = *(v8f*)&aa[0][base];
            v8f zero_vec = (v8f){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
            v8i cmp_result = (v8i)(a0_vec > zero_vec);
            mask_vec[vi] = cmp_result;
        }

        // Process columns with vectorized mask-based computation
        for (int j = 1; j < 256; j++) {
            for (int vi = 0; vi < 32; vi++) {
                int base = vi * 8;
                v8i mask = mask_vec[vi];

                // Load data for vector lanes
                v8f aa_prev = *(v8f*)&aa[j-1][base];
                v8f bb_curr = *(v8f*)&bb[j][base];
                v8f cc_curr = *(v8f*)&cc[j][base];

                // Compute new values for all lanes
                v8f new_aa = aa_prev + bb_curr * cc_curr;

                // Blend with previous values based on mask
                v8f aa_curr = *(v8f*)&aa[j][base];
                v8f result = (v8f)((mask & (v8i)new_aa) | (~mask & (v8i)aa_curr));

                // Store result
                *(v8f*)&aa[j][base] = result;
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
