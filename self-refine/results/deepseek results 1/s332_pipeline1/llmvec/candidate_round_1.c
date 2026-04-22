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
        value = -1.;
        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > t) {
                index = i;
                value = a[i];
                goto L20;
            }
        }
L20:
        chksum = value + (float) index;
    }
    return value;
}

        #include <stdint.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a)
{
    int index;
    float value;
    float chksum;
    float ft = (float)t;

    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.f;

        // Vectorized search: find first i where a[i] > t
        // We'll process in chunks of 8 to find the first match
        int found = 0;
        int vec_width = 8;
        int i = 0;

        // Process chunks of 8
        for (; i <= LEN_1D - vec_width && !found; i += vec_width) {
            // Check if any element in this chunk satisfies a[i] > t
            int any = 0;
            for (int j = 0; j < vec_width; j++) {
                if (a[i + j] > ft) {
                    any = 1;
                    break;
                }
            }
            if (any) {
                // Find the first one in this chunk
                for (int j = 0; j < vec_width; j++) {
                    if (a[i + j] > ft) {
                        index = i + j;
                        value = a[i + j];
                        found = 1;
                        break;
                    }
                }
            }
        }

        // Scalar cleanup tail
        if (!found) {
            for (; i < LEN_1D; i++) {
                if (a[i] > ft) {
                    index = i;
                    value = a[i];
                    found = 1;
                    break;
                }
            }
        }

        chksum = value + (float)index;
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
