#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s315(int iterations,int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x, chksum;
    int index;
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;
        for (int i = 0; i < LEN_1D; ++i) {
            if (a[i] > x) {
                x = a[i];
                index = i;
            }
        }
        chksum = x + (float) index;
    }
    return index + x + 1;
}

        #include <stdint.h>
typedef float v4sf __attribute__((__vector_size__(16)));

float vectorized_s315(int iterations, int LEN_1D, float* a) {
    // First loop: initialize a[i] = (i * 7) % LEN_1D
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (float)((i * 7) % LEN_1D);
    }

    float x, chksum;
    int index;
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // Vectorized max-finding loop
        int i = 0;
        if (LEN_1D >= 4) {
            v4sf vmax = {a[0], a[0], a[0], a[0]};
            int vindex[4] = {0, 0, 0, 0};
            for (; i <= LEN_1D - 4; i += 4) {
                v4sf va = *(v4sf*)(a + i);
                v4sf cmp = va > vmax;
                // Update max values
                vmax = __builtin_shufflevector(vmax, va, 0, 1, 2, 3);
                // Update indices
                for (int j = 0; j < 4; j++) {
                    if (va[j] > vmax[j]) {
                        vmax[j] = va[j];
                        vindex[j] = i + j;
                    }
                }
            }
            // Reduce vector to scalar
            for (int j = 0; j < 4; j++) {
                if (vmax[j] > x) {
                    x = vmax[j];
                    index = vindex[j];
                }
            }
        }
        // Scalar cleanup
        for (; i < LEN_1D; ++i) {
            if (a[i] > x) {
                x = a[i];
                index = i;
            }
        }
        chksum = x + (float)index;
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
