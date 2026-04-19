#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s352(int iterations,int LEN_1D, float* a, float *b) {
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.f;

        // Process in groups of 5 to match original semantics exactly
        int i;
        for (i = 0; i <= LEN_1D - 5; i += 5) {
            float t0 = a[i] * b[i];
            float t1 = a[i+1] * b[i+1];
            float t2 = a[i+2] * b[i+2];
            float t3 = a[i+3] * b[i+3];
            float t4 = a[i+4] * b[i+4];
            dot += t0 + t1 + t2 + t3 + t4;
        }

        // Handle remaining elements (less than 5)
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}

        float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.f;

        typedef float v4f __attribute__((vector_size(16)));
        v4f dot_acc0 = {0.f, 0.f, 0.f, 0.f};
        v4f dot_acc1 = {0.f, 0.f, 0.f, 0.f};
        v4f dot_acc2 = {0.f, 0.f, 0.f, 0.f};
        v4f dot_acc3 = {0.f, 0.f, 0.f, 0.f};
        v4f dot_acc4 = {0.f, 0.f, 0.f, 0.f};

        int i = 0;
        for (; i <= LEN_1D - 20; i += 20) {
            v4f a_vec0 = *(v4f*)&a[i];
            v4f b_vec0 = *(v4f*)&b[i];
            dot_acc0 += a_vec0 * b_vec0;

            v4f a_vec1 = *(v4f*)&a[i+4];
            v4f b_vec1 = *(v4f*)&b[i+4];
            dot_acc1 += a_vec1 * b_vec1;

            v4f a_vec2 = *(v4f*)&a[i+8];
            v4f b_vec2 = *(v4f*)&b[i+8];
            dot_acc2 += a_vec2 * b_vec2;

            v4f a_vec3 = *(v4f*)&a[i+12];
            v4f b_vec3 = *(v4f*)&b[i+12];
            dot_acc3 += a_vec3 * b_vec3;

            v4f a_vec4 = *(v4f*)&a[i+16];
            v4f b_vec4 = *(v4f*)&b[i+16];
            dot_acc4 += a_vec4 * b_vec4;
        }

        float* acc0 = (float*)&dot_acc0;
        float* acc1 = (float*)&dot_acc1;
        float* acc2 = (float*)&dot_acc2;
        float* acc3 = (float*)&dot_acc3;
        float* acc4 = (float*)&dot_acc4;

        dot = acc0[0] + acc0[1] + acc0[2] + acc0[3] +
              acc1[0] + acc1[1] + acc1[2] + acc1[3] +
              acc2[0] + acc2[1] + acc2[2] + acc2[3] +
              acc3[0] + acc3[1] + acc3[2] + acc3[3] +
              acc4[0] + acc4[1] + acc4[2] + acc4[3];

        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
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
