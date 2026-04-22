#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s318(int iterations,int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;

    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);

        // First pass: find maximum absolute value (reduction)
        float max_val = max;
        for (int i = 1; i < LEN_1D; i++) {
            k += inc;
            float abs_val = fabsf(a[k]);
            if (abs_val > max_val) {
                max_val = abs_val;
            }
        }

        // Second pass: find first index where value equals max_val
        k = 0;
        int found_idx = 0;
        for (int i = 0; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            if (abs_val == max_val && found_idx == 0) {
                found_idx = i;
            }
            k += inc;
        }

        max = max_val;
        index = found_idx;
        chksum = max + (float) index;
    }
    return max + index + 1;
}

        #include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;

    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);

        // First pass: find maximum absolute value (reduction)
        float max_val = max;

        // Vectorized reduction for maximum absolute value
        int i = 1;
        typedef float v4f __attribute__((vector_size(16)));
        v4f vmax = {max_val, max_val, max_val, max_val};

        // Process in chunks of 4
        for (; i + 3 < LEN_1D; i += 4) {
            k += inc;
            float abs_val0 = fabsf(a[k]);
            k += inc;
            float abs_val1 = fabsf(a[k]);
            k += inc;
            float abs_val2 = fabsf(a[k]);
            k += inc;
            float abs_val3 = fabsf(a[k]);

            v4f vabs = {abs_val0, abs_val1, abs_val2, abs_val3};
            v4f vcmp = vabs > vmax;

            // Manual blend: vmax = vcmp ? vabs : vmax
            int cmp_mask0 = vcmp[0] != 0;
            int cmp_mask1 = vcmp[1] != 0;
            int cmp_mask2 = vcmp[2] != 0;
            int cmp_mask3 = vcmp[3] != 0;

            vmax[0] = cmp_mask0 ? vabs[0] : vmax[0];
            vmax[1] = cmp_mask1 ? vabs[1] : vmax[1];
            vmax[2] = cmp_mask2 ? vabs[2] : vmax[2];
            vmax[3] = cmp_mask3 ? vabs[3] : vmax[3];
        }

        // Extract horizontal maximum from vector
        float vmax_arr[4];
        vmax_arr[0] = vmax[0];
        vmax_arr[1] = vmax[1];
        vmax_arr[2] = vmax[2];
        vmax_arr[3] = vmax[3];

        for (int j = 0; j < 4; j++) {
            if (vmax_arr[j] > max_val) {
                max_val = vmax_arr[j];
            }
        }

        // Scalar tail for first pass
        for (; i < LEN_1D; i++) {
            k += inc;
            float abs_val = fabsf(a[k]);
            if (abs_val > max_val) {
                max_val = abs_val;
            }
        }

        // Second pass: find first index where value equals max_val
        k = 0;
        int found_idx = 0;
        int found = 0;

        // Vectorized search for first occurrence
        i = 0;
        for (; i + 3 < LEN_1D && !found; i += 4) {
            // Store k at start of chunk
            int chunk_start_k = k;

            // Check first element
            float abs_val0 = fabsf(a[k]);
            if (abs_val0 == max_val) {
                found_idx = i;
                found = 1;
                k = chunk_start_k; // Reset k to found element's position
                continue;
            }
            k += inc;

            // Check second element
            float abs_val1 = fabsf(a[k]);
            if (!found && abs_val1 == max_val) {
                found_idx = i + 1;
                found = 1;
                k = chunk_start_k + inc; // k points to found element
                continue;
            }
            k += inc;

            // Check third element
            float abs_val2 = fabsf(a[k]);
            if (!found && abs_val2 == max_val) {
                found_idx = i + 2;
                found = 1;
                k = chunk_start_k + 2 * inc; // k points to found element
                continue;
            }
            k += inc;

            // Check fourth element
            float abs_val3 = fabsf(a[k]);
            if (!found && abs_val3 == max_val) {
                found_idx = i + 3;
                found = 1;
                k = chunk_start_k + 3 * inc; // k points to found element
                continue;
            }

            // No match in this chunk, advance to next chunk
            k += inc;
        }

        // Scalar tail for second pass
        for (; i < LEN_1D && !found; i++) {
            float abs_val = fabsf(a[k]);
            if (abs_val == max_val) {
                found_idx = i;
                found = 1;
                break;
            }
            k += inc;
        }

        max = max_val;
        index = found_idx;
        chksum = max + (float) index;
    }
    return max + index + 1;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; int inc = 7; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s318(iterations, LEN_1D, a_scalar, inc); ret_vector = vectorized_s318(iterations, LEN_1D, a_vector, inc); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
