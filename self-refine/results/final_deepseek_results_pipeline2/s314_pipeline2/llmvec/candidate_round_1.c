#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s314(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            float current = a[i];
            x = (current > x) ? current : x;
        }
    }
    return x;
}

        typedef float float4 __attribute__((ext_vector_type(4)));

float vectorized_s314(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        x = a[0];

        // Vectorized main loop
        int i = 0;
        if (LEN_1D >= 4) {
            float4 vmax = (float4){x, x, x, x};
            for (; i <= LEN_1D - 4; i += 4) {
                float4 vcurrent = *(float4*)&a[i];

                // Component-wise maximum
                vmax = (float4){
                    vcurrent.x > vmax.x ? vcurrent.x : vmax.x,
                    vcurrent.y > vmax.y ? vcurrent.y : vmax.y,
                    vcurrent.z > vmax.z ? vcurrent.z : vmax.z,
                    vcurrent.w > vmax.w ? vcurrent.w : vmax.w
                };
            }

            // Reduce vector maximum to scalar
            float vmax_reduced = vmax.x;
            if (vmax.y > vmax_reduced) vmax_reduced = vmax.y;
            if (vmax.z > vmax_reduced) vmax_reduced = vmax.z;
            if (vmax.w > vmax_reduced) vmax_reduced = vmax.w;
            x = vmax_reduced;
        }

        // Scalar tail cleanup
        for (; i < LEN_1D; i++) {
            float current = a[i];
            x = (current > x) ? current : x;
        }
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
