#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s442(int iterations,int LEN_1D, float* a, float *b,float* c, float *d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            switch (indx[i]) {
                case 1:  goto L15;
                case 2:  goto L20;
                case 3:  goto L30;
                case 4:  goto L40;
            }
L15:
            a[i] += b[i] * b[i];
            goto L50;
L20:
            a[i] += c[i] * c[i];
            goto L50;
L30:
            a[i] += d[i] * d[i];
            goto L50;
L40:
            a[i] += e[i] * e[i];
L50:
            ;
        }
    }
}

        void vectorized_s442(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx)
{
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        /* Process 4 elements at a time using unrolled scalar logic */
        for (; i <= LEN_1D - 4; i += 4) {
            /* Element i+0 */
            {
                int idx = indx[i];
                if (idx == 1)      a[i] += b[i] * b[i];
                else if (idx == 2) a[i] += c[i] * c[i];
                else if (idx == 3) a[i] += d[i] * d[i];
                else if (idx == 4) a[i] += e[i] * e[i];
            }
            /* Element i+1 */
            {
                int idx = indx[i+1];
                if (idx == 1)      a[i+1] += b[i+1] * b[i+1];
                else if (idx == 2) a[i+1] += c[i+1] * c[i+1];
                else if (idx == 3) a[i+1] += d[i+1] * d[i+1];
                else if (idx == 4) a[i+1] += e[i+1] * e[i+1];
            }
            /* Element i+2 */
            {
                int idx = indx[i+2];
                if (idx == 1)      a[i+2] += b[i+2] * b[i+2];
                else if (idx == 2) a[i+2] += c[i+2] * c[i+2];
                else if (idx == 3) a[i+2] += d[i+2] * d[i+2];
                else if (idx == 4) a[i+2] += e[i+2] * e[i+2];
            }
            /* Element i+3 */
            {
                int idx = indx[i+3];
                if (idx == 1)      a[i+3] += b[i+3] * b[i+3];
                else if (idx == 2) a[i+3] += c[i+3] * c[i+3];
                else if (idx == 3) a[i+3] += d[i+3] * d[i+3];
                else if (idx == 4) a[i+3] += e[i+3] * e[i+3];
            }
        }
        /* Scalar cleanup tail */
        for (; i < LEN_1D; i++) {
            switch (indx[i]) {
                case 1: a[i] += b[i] * b[i]; break;
                case 2: a[i] += c[i] * c[i]; break;
                case 3: a[i] += d[i] * d[i]; break;
                case 4: a[i] += e[i] * e[i]; break;
                default: break;
            }
        }
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128]; int indx_scalar[128]; int indx_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar)); fill_i32(indx_scalar, n, &seed); memcpy(indx_vector, indx_scalar, sizeof(indx_scalar));
                s442(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar, indx_scalar); vectorized_s442(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector, indx_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if ((indx_scalar[i]) != (indx_vector[i])) {
        fprintf(stderr, "Mismatch in parameter indx on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
