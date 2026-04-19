#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s332(int iterations,int LEN_1D,int t, float* a)
{
    float value = 0.0f;
    float chksum = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        int index = -2;
        float current_value = -1.0f;

        int i = 0;
        int found_index = LEN_1D;

        // Main vectorizable loop - process in chunks
        for (; i + 3 < LEN_1D; i += 4) {
            // Load 4 elements
            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];

            // Compute comparisons without branching
            int cond0 = a0 > t;
            int cond1 = a1 > t;
            int cond2 = a2 > t;
            int cond3 = a3 > t;

            // Calculate first match index in this chunk using arithmetic
            int first_in_chunk = LEN_1D;

            // Branchless calculation of first true index
            int mask0 = cond0;
            int mask1 = cond1 && !mask0;
            int mask2 = cond2 && !mask0 && !mask1;
            int mask3 = cond3 && !mask0 && !mask1 && !mask2;

            first_in_chunk = mask0 * i + mask1 * (i+1) + mask2 * (i+2) + mask3 * (i+3) + 
                           (!mask0 && !mask1 && !mask2 && !mask3) * LEN_1D;

            // Update global minimum index
            if (first_in_chunk < found_index) {
                found_index = first_in_chunk;
            }
        }

        // Remainder loop for cleanup
        for (; i < LEN_1D; i++) {
            if (a[i] > t) {
                if (i < found_index) {
                    found_index = i;
                }
                break;
            }
        }

        // Finalize results
        if (found_index < LEN_1D) {
            index = found_index;
            current_value = a[found_index];
        }

        value = current_value;
        chksum = value + (float)index;
    }
    return value;
}

        #include <stdint.h>
#include <string.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a) {
    float value = 0.0f;
    float chksum = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        int index = -2;
        float current_value = -1.0f;
        int found_index = LEN_1D;

        // Vectorized main loop
        int i = 0;
        for (; i + 7 < LEN_1D; i += 8) {
            // Load 8 elements
            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];
            float a4 = a[i+4];
            float a5 = a[i+5];
            float a6 = a[i+6];
            float a7 = a[i+7];

            // Compute comparisons
            int cond0 = a0 > t;
            int cond1 = a1 > t;
            int cond2 = a2 > t;
            int cond3 = a3 > t;
            int cond4 = a4 > t;
            int cond5 = a5 > t;
            int cond6 = a6 > t;
            int cond7 = a7 > t;

            // Find first true in this chunk
            int first_in_chunk = LEN_1D;

            // Process first 4 elements
            int mask0 = cond0;
            int mask1 = cond1 && !mask0;
            int mask2 = cond2 && !mask0 && !mask1;
            int mask3 = cond3 && !mask0 && !mask1 && !mask2;

            int first_half = mask0 * i + mask1 * (i+1) + mask2 * (i+2) + mask3 * (i+3);
            int first_half_valid = mask0 || mask1 || mask2 || mask3;

            // Process next 4 elements only if no match in first half
            int mask4 = cond4 && !first_half_valid;
            int mask5 = cond5 && !first_half_valid && !mask4;
            int mask6 = cond6 && !first_half_valid && !mask4 && !mask5;
            int mask7 = cond7 && !first_half_valid && !mask4 && !mask5 && !mask6;

            int second_half = mask4 * (i+4) + mask5 * (i+5) + mask6 * (i+6) + mask7 * (i+7);
            int second_half_valid = mask4 || mask5 || mask6 || mask7;

            if (first_half_valid) {
                first_in_chunk = first_half;
            } else if (second_half_valid) {
                first_in_chunk = second_half;
            }

            // Update global minimum
            if (first_in_chunk < found_index) {
                found_index = first_in_chunk;
                // Early exit if we found a match at the beginning of the chunk
                if (found_index == i) {
                    i = LEN_1D; // Force exit from vector loop
                    break;
                }
            }
        }

        // Scalar remainder loop
        for (; i < LEN_1D; i++) {
            if (a[i] > t) {
                if (i < found_index) {
                    found_index = i;
                }
                break;
            }
        }

        // Finalize results
        if (found_index < LEN_1D) {
            index = found_index;
            current_value = a[found_index];
        }

        value = current_value;
        chksum = value + (float)index;
    }
    return value;
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
            int iterations = 5; int LEN_1D = arr_len; int t = 7; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s332(iterations, LEN_1D, t, a_scalar); ret_vector = vectorized_s332(iterations, LEN_1D, t, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
