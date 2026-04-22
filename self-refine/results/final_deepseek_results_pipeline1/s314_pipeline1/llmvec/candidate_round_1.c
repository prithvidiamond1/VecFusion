#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s314(int iterations,int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > x) {
                x = a[i];
            }
        }
    }
    return x;
}

        #include <stdint.h>
typedef float v4f __attribute__((ext_vector_type(4)));

float vectorized_s314(int iterations, int LEN_1D, float* a) {
    float x;
    int total_outer = iterations * 5;
    for (int nl = 0; nl < total_outer; nl++) {
        x = a[0];
        int i = 0;
        // Vectorized loop: process 4 elements at a time
        v4f vmax = (v4f){x, x, x, x};
        for (; i + 4 <= LEN_1D; i += 4) {
            v4f va = *(v4f*)(a + i);
            v4f mask = va > vmax;
            vmax = __builtin_shufflevector(vmax, va, 0, 1, 2, 3);
            // Blend: where mask is true, take va; else keep vmax
            vmax = (v4f)((__builtin_convertvector(mask, v4f_signed) & (int32_t)va) | (~__builtin_convertvector(mask, v4f_signed) & (int32_t)vmax));
            // Actually simpler: use conditional select
            vmax = (va > vmax) ? va : vmax;
        }
        // Horizontal max of vmax
        float max0 = vmax[0];
        float max1 = vmax[1];
        float max2 = vmax[2];
        float max3 = vmax[3];
        float hmax = max0;
        if (max1 > hmax) hmax = max1;
        if (max2 > hmax) hmax = max2;
        if (max3 > hmax) hmax = max3;
        // Scalar tail
        for (; i < LEN_1D; i++) {
            if (a[i] > hmax) {
                hmax = a[i];
            }
        }
        x = hmax;
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
                ret_scalar = s314(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s314(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
