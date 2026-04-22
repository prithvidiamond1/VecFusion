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
        value = -1.0f;

        // Split: search loop without goto
        int found_index = -2;
        float found_value = -1.0f;

        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > t) {
                found_index = i;
                found_value = a[i];
                break;
            }
        }

        // Assignment after search completes
        index = found_index;
        value = found_value;

        chksum = value + (float)index;
    }
    return value;
}

        #include <stdint.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a)
{
    int index;
    float value;
    float chksum;

    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;

        int found_index = -2;
        float found_value = -1.0f;

        float ft = (float)t;

        // Process 8 elements at a time to find first index where a[i] > t
        int i = 0;
        int vec_width = 8;
        int limit = LEN_1D - (LEN_1D % vec_width);

        // We need to find the FIRST occurrence, so we process chunks
        // but must check each chunk carefully
        int done = 0;
        for (i = 0; i < limit && !done; i += vec_width) {
            // Check if any element in this chunk satisfies condition
            int any = (a[i+0] > ft) | (a[i+1] > ft) | (a[i+2] > ft) | (a[i+3] > ft) |
                      (a[i+4] > ft) | (a[i+5] > ft) | (a[i+6] > ft) | (a[i+7] > ft);
            if (any) {
                // Find the first one in this chunk
                for (int j = i; j < i + vec_width; j++) {
                    if (a[j] > ft) {
                        found_index = j;
                        found_value = a[j];
                        done = 1;
                        break;
                    }
                }
            }
        }

        // Scalar cleanup tail
        if (!done) {
            for (i = limit; i < LEN_1D; i++) {
                if (a[i] > ft) {
                    found_index = i;
                    found_value = a[i];
                    break;
                }
            }
        }

        index = found_index;
        value = found_value;

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
