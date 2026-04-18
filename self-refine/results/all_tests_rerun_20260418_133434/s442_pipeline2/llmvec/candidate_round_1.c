#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s442(int iterations,int LEN_1D, float* a, float *b,float* c, float *d, float* e, int* indx)
{
    // Precompute indices for each case
    int idx1[LEN_1D], idx2[LEN_1D], idx3[LEN_1D], idx4[LEN_1D];
    int cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0;

    for (int i = 0; i < LEN_1D; i++) {
        switch (indx[i]) {
            case 1: idx1[cnt1++] = i; break;
            case 2: idx2[cnt2++] = i; break;
            case 3: idx3[cnt3++] = i; break;
            case 4: idx4[cnt4++] = i; break;
        }
    }

    for (int nl = 0; nl < iterations/2; nl++) {
        // Process each case without conditionals
        for (int j = 0; j < cnt1; j++) {
            int i = idx1[j];
            a[i] += b[i] * b[i];
        }
        for (int j = 0; j < cnt2; j++) {
            int i = idx2[j];
            a[i] += c[i] * c[i];
        }
        for (int j = 0; j < cnt3; j++) {
            int i = idx3[j];
            a[i] += d[i] * d[i];
        }
        for (int j = 0; j < cnt4; j++) {
            int i = idx4[j];
            a[i] += e[i] * e[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s442(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx)
{
    // Precompute indices for each case
    int idx1[LEN_1D], idx2[LEN_1D], idx3[LEN_1D], idx4[LEN_1D];
    int cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0;

    for (int i = 0; i < LEN_1D; i++) {
        switch (indx[i]) {
            case 1: idx1[cnt1++] = i; break;
            case 2: idx2[cnt2++] = i; break;
            case 3: idx3[cnt3++] = i; break;
            case 4: idx4[cnt4++] = i; break;
        }
    }

    for (int nl = 0; nl < iterations/2; nl++) {
        // Process case 1: a[i] += b[i] * b[i]
        {
            int j = 0;
            int limit = cnt1 - (cnt1 % 4);
            for (; j < limit; j += 4) {
                int i0 = idx1[j+0];
                int i1 = idx1[j+1];
                int i2 = idx1[j+2];
                int i3 = idx1[j+3];
                a[i0] += b[i0] * b[i0];
                a[i1] += b[i1] * b[i1];
                a[i2] += b[i2] * b[i2];
                a[i3] += b[i3] * b[i3];
            }
            for (; j < cnt1; j++) {
                int i = idx1[j];
                a[i] += b[i] * b[i];
            }
        }

        // Process case 2: a[i] += c[i] * c[i]
        {
            int j = 0;
            int limit = cnt2 - (cnt2 % 4);
            for (; j < limit; j += 4) {
                int i0 = idx2[j+0];
                int i1 = idx2[j+1];
                int i2 = idx2[j+2];
                int i3 = idx2[j+3];
                a[i0] += c[i0] * c[i0];
                a[i1] += c[i1] * c[i1];
                a[i2] += c[i2] * c[i2];
                a[i3] += c[i3] * c[i3];
            }
            for (; j < cnt2; j++) {
                int i = idx2[j];
                a[i] += c[i] * c[i];
            }
        }

        // Process case 3: a[i] += d[i] * d[i]
        {
            int j = 0;
            int limit = cnt3 - (cnt3 % 4);
            for (; j < limit; j += 4) {
                int i0 = idx3[j+0];
                int i1 = idx3[j+1];
                int i2 = idx3[j+2];
                int i3 = idx3[j+3];
                a[i0] += d[i0] * d[i0];
                a[i1] += d[i1] * d[i1];
                a[i2] += d[i2] * d[i2];
                a[i3] += d[i3] * d[i3];
            }
            for (; j < cnt3; j++) {
                int i = idx3[j];
                a[i] += d[i] * d[i];
            }
        }

        // Process case 4: a[i] += e[i] * e[i]
        {
            int j = 0;
            int limit = cnt4 - (cnt4 % 4);
            for (; j < limit; j += 4) {
                int i0 = idx4[j+0];
                int i1 = idx4[j+1];
                int i2 = idx4[j+2];
                int i3 = idx4[j+3];
                a[i0] += e[i0] * e[i0];
                a[i1] += e[i1] * e[i1];
                a[i2] += e[i2] * e[i2];
                a[i3] += e[i3] * e[i3];
            }
            for (; j < cnt4; j++) {
                int i = idx4[j];
                a[i] += e[i] * e[i];
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
