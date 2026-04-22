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
    for (int nl = 0; nl < iterations/2; nl++) {
        int i = 0;
        /* Process 4 elements at a time */
        for (; i <= LEN_1D - 4; i += 4) {
            /* Element i+0 */
            {
                int idx = indx[i];
                float val;
                if (idx == 1) { val = b[i]; a[i] += val * val; }
                else if (idx == 2) { val = c[i]; a[i] += val * val; }
                else if (idx == 3) { val = d[i]; a[i] += val * val; }
                else if (idx == 4) { val = e[i]; a[i] += val * val; }
                else {
                    /* fall-through: same as case 1 label L15 reached by default */
                    val = b[i]; a[i] += val * val;
                }
            }
            /* Element i+1 */
            {
                int idx = indx[i+1];
                float val;
                if (idx == 1) { val = b[i+1]; a[i+1] += val * val; }
                else if (idx == 2) { val = c[i+1]; a[i+1] += val * val; }
                else if (idx == 3) { val = d[i+1]; a[i+1] += val * val; }
                else if (idx == 4) { val = e[i+1]; a[i+1] += val * val; }
                else {
                    val = b[i+1]; a[i+1] += val * val;
                }
            }
            /* Element i+2 */
            {
                int idx = indx[i+2];
                float val;
                if (idx == 1) { val = b[i+2]; a[i+2] += val * val; }
                else if (idx == 2) { val = c[i+2]; a[i+2] += val * val; }
                else if (idx == 3) { val = d[i+2]; a[i+2] += val * val; }
                else if (idx == 4) { val = e[i+2]; a[i+2] += val * val; }
                else {
                    val = b[i+2]; a[i+2] += val * val;
                }
            }
            /* Element i+3 */
            {
                int idx = indx[i+3];
                float val;
                if (idx == 1) { val = b[i+3]; a[i+3] += val * val; }
                else if (idx == 2) { val = c[i+3]; a[i+3] += val * val; }
                else if (idx == 3) { val = d[i+3]; a[i+3] += val * val; }
                else if (idx == 4) { val = e[i+3]; a[i+3] += val * val; }
                else {
                    val = b[i+3]; a[i+3] += val * val;
                }
            }
        }
        /* Scalar cleanup */
        for (; i < LEN_1D; i++) {
            int idx = indx[i];
            float val;
            if (idx == 1) { val = b[i]; a[i] += val * val; }
            else if (idx == 2) { val = c[i]; a[i] += val * val; }
            else if (idx == 3) { val = d[i]; a[i] += val * val; }
            else if (idx == 4) { val = e[i]; a[i] += val * val; }
            else {
                val = b[i]; a[i] += val * val;
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128]; int indx_scalar[128]; int indx_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar)); fill_i32(indx_scalar, arr_len, &seed); memcpy(indx_vector, indx_scalar, sizeof(indx_scalar));
                s442(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar, indx_scalar); vectorized_s442(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector, indx_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if ((indx_scalar[i]) != (indx_vector[i])) {
        fprintf(stderr, "Mismatch in parameter indx on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
