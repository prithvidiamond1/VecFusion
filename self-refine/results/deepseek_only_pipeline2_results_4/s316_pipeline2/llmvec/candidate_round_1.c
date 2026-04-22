#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s316(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float min1 = a[0];
        float min2 = a[0];
        int i;

        // Process elements in pairs to reduce loop-carried dependency
        for (i = 1; i < LEN_1D-1; i += 2) {
            float val1 = a[i];
            float val2 = a[i+1];

            // Compare first element
            if (val1 < min1) {
                min1 = val1;
            }

            // Compare second element
            if (val2 < min2) {
                min2 = val2;
            }
        }

        // Handle remaining element if LEN_1D is odd
        if (i < LEN_1D) {
            float val = a[i];
            if (val < min1) {
                min1 = val;
            }
        }

        // Final reduction
        x = (min1 < min2) ? min1 : min2;
    }
    return x;
}

        #include <stddef.h>

float vectorized_s316(int iterations, int LEN_1D, float* a) {
    float x = 0.0f;

    for (int nl = 0; nl < iterations * 5; nl++) {
        float min1 = a[0];
        float min2 = a[0];

        // Vectorized processing
        int i = 1;
        for (; i + 3 < LEN_1D; i += 4) {
            float v0 = a[i];
            float v1 = a[i + 1];
            float v2 = a[i + 2];
            float v3 = a[i + 3];

            // Process in pairs to maintain the original pattern
            if (v0 < min1) min1 = v0;
            if (v1 < min2) min2 = v1;
            if (v2 < min1) min1 = v2;
            if (v3 < min2) min2 = v3;
        }

        // Handle remaining elements in pairs
        for (; i + 1 < LEN_1D; i += 2) {
            float val1 = a[i];
            float val2 = a[i + 1];

            if (val1 < min1) min1 = val1;
            if (val2 < min2) min2 = val2;
        }

        // Handle last element if LEN_1D is odd
        if (i < LEN_1D) {
            float val = a[i];
            if (val < min1) min1 = val;
        }

        // Final reduction
        x = (min1 < min2) ? min1 : min2;
    }

    return x;
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
                ret_scalar = s316(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s316(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
