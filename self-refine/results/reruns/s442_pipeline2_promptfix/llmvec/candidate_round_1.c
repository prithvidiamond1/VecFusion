#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s442(int iterations,int LEN_1D, float* a, float *b,float* c, float *d, float* e, int* indx)
{
    int nl, i;
    int iter = iterations / 2;

    // Precompute indices for each case to eliminate conditionals in hot loops
    int idx1[LEN_1D], idx2[LEN_1D], idx3[LEN_1D], idx4[LEN_1D];
    int count1 = 0, count2 = 0, count3 = 0, count4 = 0;

    for (i = 0; i < LEN_1D; i++) {
        switch (indx[i]) {
            case 1: idx1[count1++] = i; break;
            case 2: idx2[count2++] = i; break;
            case 3: idx3[count3++] = i; break;
            case 4: idx4[count4++] = i; break;
        }
    }

    for (nl = 0; nl < iter; nl++) {
        // Process each case with contiguous memory access patterns
        for (int j = 0; j < count1; j++) {
            i = idx1[j];
            a[i] += b[i] * b[i];
        }
        for (int j = 0; j < count2; j++) {
            i = idx2[j];
            a[i] += c[i] * c[i];
        }
        for (int j = 0; j < count3; j++) {
            i = idx3[j];
            a[i] += d[i] * d[i];
        }
        for (int j = 0; j < count4; j++) {
            i = idx4[j];
            a[i] += e[i] * e[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s442(int iterations, int LEN_1D, float* a, float *b, float* c, float *d, float* e, int* indx)
{
    int nl, i;
    int iter = iterations / 2;

    int idx1[LEN_1D], idx2[LEN_1D], idx3[LEN_1D], idx4[LEN_1D];
    int count1 = 0, count2 = 0, count3 = 0, count4 = 0;

    for (i = 0; i < LEN_1D; i++) {
        switch (indx[i]) {
            case 1: idx1[count1++] = i; break;
            case 2: idx2[count2++] = i; break;
            case 3: idx3[count3++] = i; break;
            case 4: idx4[count4++] = i; break;
        }
    }

    for (nl = 0; nl < iter; nl++) {
        // Case 1: a[i] += b[i] * b[i]
        {
            int j = 0;
            int limit1 = count1 - (count1 % 4);
            for (; j < limit1; j += 4) {
                int i0 = idx1[j+0];
                int i1 = idx1[j+1];
                int i2 = idx1[j+2];
                int i3 = idx1[j+3];
                a[i0] += b[i0] * b[i0];
                a[i1] += b[i1] * b[i1];
                a[i2] += b[i2] * b[i2];
                a[i3] += b[i3] * b[i3];
            }
            for (; j < count1; j++) {
                int ii = idx1[j];
                a[ii] += b[ii] * b[ii];
            }
        }

        // Case 2: a[i] += c[i] * c[i]
        {
            int j = 0;
            int limit2 = count2 - (count2 % 4);
            for (; j < limit2; j += 4) {
                int i0 = idx2[j+0];
                int i1 = idx2[j+1];
                int i2 = idx2[j+2];
                int i3 = idx2[j+3];
                a[i0] += c[i0] * c[i0];
                a[i1] += c[i1] * c[i1];
                a[i2] += c[i2] * c[i2];
                a[i3] += c[i3] * c[i3];
            }
            for (; j < count2; j++) {
                int ii = idx2[j];
                a[ii] += c[ii] * c[ii];
            }
        }

        // Case 3: a[i] += d[i] * d[i]
        {
            int j = 0;
            int limit3 = count3 - (count3 % 4);
            for (; j < limit3; j += 4) {
                int i0 = idx3[j+0];
                int i1 = idx3[j+1];
                int i2 = idx3[j+2];
                int i3 = idx3[j+3];
                a[i0] += d[i0] * d[i0];
                a[i1] += d[i1] * d[i1];
                a[i2] += d[i2] * d[i2];
                a[i3] += d[i3] * d[i3];
            }
            for (; j < count3; j++) {
                int ii = idx3[j];
                a[ii] += d[ii] * d[ii];
            }
        }

        // Case 4: a[i] += e[i] * e[i]
        {
            int j = 0;
            int limit4 = count4 - (count4 % 4);
            for (; j < limit4; j += 4) {
                int i0 = idx4[j+0];
                int i1 = idx4[j+1];
                int i2 = idx4[j+2];
                int i3 = idx4[j+3];
                a[i0] += e[i0] * e[i0];
                a[i1] += e[i1] * e[i1];
                a[i2] += e[i2] * e[i2];
                a[i3] += e[i3] * e[i3];
            }
            for (; j < count4; j++) {
                int ii = idx4[j];
                a[ii] += e[ii] * e[ii];
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
