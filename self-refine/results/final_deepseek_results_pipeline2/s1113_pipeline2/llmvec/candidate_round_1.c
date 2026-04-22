#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1113(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float a_mid_original = a[mid];
        float updated_mid = a_mid_original + b[mid];

        for (int i = 0; i < mid; i++) {
            a[i] = a_mid_original + b[i];
        }

        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = updated_mid + b[i];
        }

        a[mid] = updated_mid;
    }
}

        typedef float v8f __attribute__((__vector_size__(32), __aligned__(4)));

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;
    int outer_iterations = 2 * iterations;

    for (int nl = 0; nl < outer_iterations; nl++) {
        float a_mid_original = a[mid];
        float updated_mid = a_mid_original + b[mid];

        // Vectorize first loop: i = 0 to mid-1
        int i = 0;
        int first_loop_bound = mid;
        v8f a_mid_vec = (v8f){a_mid_original, a_mid_original, a_mid_original, a_mid_original,
                              a_mid_original, a_mid_original, a_mid_original, a_mid_original};

        for (; i + 7 < first_loop_bound; i += 8) {
            v8f b_vec = *(v8f*)&b[i];
            v8f result_vec = a_mid_vec + b_vec;
            *(v8f*)&a[i] = result_vec;
        }

        // Scalar tail for first loop
        for (; i < first_loop_bound; i++) {
            a[i] = a_mid_original + b[i];
        }

        // Vectorize second loop: i = mid+1 to LEN_1D-1
        i = mid + 1;
        int second_loop_bound = LEN_1D;
        v8f updated_mid_vec = (v8f){updated_mid, updated_mid, updated_mid, updated_mid,
                                    updated_mid, updated_mid, updated_mid, updated_mid};

        for (; i + 7 < second_loop_bound; i += 8) {
            v8f b_vec = *(v8f*)&b[i];
            v8f result_vec = updated_mid_vec + b_vec;
            *(v8f*)&a[i] = result_vec;
        }

        // Scalar tail for second loop
        for (; i < second_loop_bound; i++) {
            a[i] = updated_mid + b[i];
        }

        a[mid] = updated_mid;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s1113(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s1113(iterations, LEN_1D, a_vector, b_vector);
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
