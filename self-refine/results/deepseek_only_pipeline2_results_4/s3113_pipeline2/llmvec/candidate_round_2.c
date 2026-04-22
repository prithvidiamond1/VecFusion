#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s3113(int iterations,int LEN_1D, float* a)
{
    float max;
    float current_max;

    for (int nl = 0; nl < iterations*4; nl++) {
        current_max = fabsf(a[0]);

        for (int i = 0; i < LEN_1D; i++) {
            float abs_val = fabsf(a[i]);
            if (abs_val > current_max) {
                current_max = abs_val;
            }
        }

        max = current_max;
    }

    return max;
}

        #include <math.h>
#include <stdint.h>

float vectorized_s3113(int iterations, int LEN_1D, float* a) {
    float max;
    float current_max;

    for (int nl = 0; nl < iterations * 4; nl++) {
        current_max = fabsf(a[0]);

        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        const int VF = 4;

        v4f vmax = (v4f){current_max, current_max, current_max, current_max};

        for (; i + VF <= LEN_1D; i += VF) {
            v4f va = *(v4f*)&a[i];
            v4f vabs = __builtin_elementwise_abs(va);
            vmax = __builtin_elementwise_max(vabs, vmax);
        }

        float vmax_arr[VF];
        *(v4f*)vmax_arr = vmax;
        float vec_max = vmax_arr[0];
        for (int j = 1; j < VF; j++) {
            if (vmax_arr[j] > vec_max) {
                vec_max = vmax_arr[j];
            }
        }

        if (vec_max > current_max) {
            current_max = vec_max;
        }

        for (; i < LEN_1D; i++) {
            float abs_val = fabsf(a[i]);
            if (abs_val > current_max) {
                current_max = abs_val;
            }
        }

        max = current_max;
    }

    return max;
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
                ret_scalar = s3113(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s3113(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
