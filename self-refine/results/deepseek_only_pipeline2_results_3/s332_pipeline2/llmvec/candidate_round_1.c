#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s332(int iterations,int LEN_1D,int t, float* a)
{
    int index;
    float value;
    float chksum;

    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;

        // Loop distribution: separate the search for first match from value extraction
        int found_index = -2;

        // First loop: find the first index where a[i] > t (vectorizable search)
        for (int i = 0; i < LEN_1D; i++) {
            // Branchless comparison - creates mask of matches
            int match = a[i] > t;
            // Only update if we haven't found a match yet
            int update = match && (found_index == -2);
            // Use arithmetic to update index without branching
            found_index = update * i + (1 - update) * found_index;
        }

        // Second loop: extract the value if index was found
        if (found_index != -2) {
            index = found_index;
            value = a[found_index];
        }

        chksum = value + (float)index;
    }
    return value;
}

        #include <stdint.h>
#include <string.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a) {
    float value = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        int index = -2;
        value = -1.0f;

        // Vectorized search for first element > t
        int found_index = -2;
        int i = 0;

        // Process in chunks for potential vectorization
        for (; i + 3 < LEN_1D; i += 4) {
            // Check 4 elements at once
            int match0 = a[i] > t;
            int match1 = a[i + 1] > t;
            int match2 = a[i + 2] > t;
            int match3 = a[i + 3] > t;

            // Find first match in this chunk
            if (found_index == -2) {
                if (match0) {
                    found_index = i;
                } else if (match1) {
                    found_index = i + 1;
                } else if (match2) {
                    found_index = i + 2;
                } else if (match3) {
                    found_index = i + 3;
                }
            }
        }

        // Handle remaining elements
        for (; i < LEN_1D; i++) {
            if (found_index == -2 && a[i] > t) {
                found_index = i;
            }
        }

        // Extract value if index was found
        if (found_index != -2) {
            index = found_index;
            value = a[found_index];
        }

        // Final computation (kept for semantic correctness)
        float chksum = value + (float)index;
        (void)chksum; // Prevent unused variable warning
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
