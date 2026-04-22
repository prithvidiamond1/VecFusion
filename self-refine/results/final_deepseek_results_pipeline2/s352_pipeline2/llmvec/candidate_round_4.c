#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s352(int iterations,int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;

        // Create multiple partial sums to break dependency chain
        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f, dot4 = 0.0f;

        // Process groups of 5 elements with 5 independent accumulators
        int i;
        for (i = 0; i + 4 < LEN_1D; i += 5) {
            // Each accumulator handles one element from the group
            dot0 += a[i] * b[i];
            dot1 += a[i + 1] * b[i + 1];
            dot2 += a[i + 2] * b[i + 2];
            dot3 += a[i + 3] * b[i + 3];
            dot4 += a[i + 4] * b[i + 4];
        }

        // Combine partial sums
        dot = dot0 + dot1 + dot2 + dot3 + dot4;

        // Handle remaining elements (if any)
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}

        float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot = 0.0f;

    for (int nl = 0; nl < 8 * iterations; nl++) {
        float partial_dot = 0.0f;

        // Ensure main_len is non-negative
        int main_len = LEN_1D >= 0 ? LEN_1D - (LEN_1D % 8) : 0;
        if (main_len < 0) main_len = 0;

        // Vectorized processing with explicit unaligned loads
        int i = 0;
        for (; i < main_len; i += 8) {
            typedef float v8f __attribute__((vector_size(32), aligned(1)));

            // Use memcpy for safe unaligned loads
            v8f va, vb;
            __builtin_memcpy(&va, &a[i], sizeof(v8f));
            __builtin_memcpy(&vb, &b[i], sizeof(v8f));

            v8f vprod = va * vb;

            // Safe horizontal sum using union to avoid aliasing issues
            union {
                v8f vec;
                float arr[8];
            } u;
            u.vec = vprod;

            partial_dot += u.arr[0] + u.arr[1] + u.arr[2] + u.arr[3] +
                          u.arr[4] + u.arr[5] + u.arr[6] + u.arr[7];
        }

        // Scalar tail for remaining elements
        for (; i < LEN_1D; i++) {
            partial_dot += a[i] * b[i];
        }

        // Accumulate across outer iterations
        dot += partial_dot;
    }

    return dot;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                ret_scalar = s352(iterations, LEN_1D, a_scalar, b_scalar); ret_vector = vectorized_s352(iterations, LEN_1D, a_vector, b_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
