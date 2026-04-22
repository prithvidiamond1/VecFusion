#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s315(int iterations, int LEN_1D, float* a) {
    // Initialization loop - already vectorizable
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x;
    int index;

    // Main loop with preserved semantics
    for (int nl = 0; nl < iterations; nl++) {
        // Initialize reduction variables
        x = a[0];
        index = 0;

        // Split the reduction: first find max value only
        float max_val = a[0];
        for (int i = 0; i < LEN_1D; ++i) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }

        // Then find the first index where value equals max_val
        // This loop has no loop-carried dependency on index
        for (int i = 0; i < LEN_1D; ++i) {
            if (a[i] == max_val) {
                index = i;
                break;  // Get first occurrence
            }
        }

        x = max_val;
        // chksum computation (kept for semantic equivalence)
        float chksum = x + (float)index;
    }

    return index + x + 1;
}

        typedef float float4 __attribute__((ext_vector_type(4)));
typedef int int4 __attribute__((ext_vector_type(4)));

float vectorized_s315(int iterations, int LEN_1D, float* a) {
    // Initialization loop - vectorized
    int i = 0;
    for (; i + 3 < LEN_1D; i += 4) {
        int4 indices = {i, i+1, i+2, i+3};
        int4 scaled = indices * 7;
        int4 mod_result = scaled % LEN_1D;
        float4 result = {(float)mod_result[0], (float)mod_result[1],
                         (float)mod_result[2], (float)mod_result[3]};
        *(float4*)&a[i] = result;
    }
    for (; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x = 0.0f;
    int index = 0;

    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // Vectorized max reduction
        float max_val = a[0];
        i = 0;

        // Handle initial elements if LEN_1D < 4
        if (LEN_1D >= 4) {
            float4 max_vec = *(float4*)&a[0];
            i = 4;

            for (; i + 3 < LEN_1D; i += 4) {
                float4 current_vec = *(float4*)&a[i];
                int4 cmp = current_vec > max_vec;
                // Manual blend using conditional operator per lane
                max_vec[0] = cmp[0] ? current_vec[0] : max_vec[0];
                max_vec[1] = cmp[1] ? current_vec[1] : max_vec[1];
                max_vec[2] = cmp[2] ? current_vec[2] : max_vec[2];
                max_vec[3] = cmp[3] ? current_vec[3] : max_vec[3];
            }

            // Extract scalar max from vector
            max_val = max_vec[0];
            if (max_vec[1] > max_val) max_val = max_vec[1];
            if (max_vec[2] > max_val) max_val = max_vec[2];
            if (max_vec[3] > max_val) max_val = max_vec[3];
        }

        // Scalar tail for max
        for (; i < LEN_1D; i++) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }

        // Find first index where value equals max_val
        index = 0;
        for (i = 0; i < LEN_1D; ++i) {
            if (a[i] == max_val) {
                index = i;
                break;
            }
        }

        x = max_val;
        float chksum = x + (float)index;
        (void)chksum;  // Suppress unused variable warning
    }

    return index + x + 1;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s315(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s315(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
