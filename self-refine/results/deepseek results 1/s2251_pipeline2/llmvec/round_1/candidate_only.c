#include <alloca.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;

        // Precompute all s values
        float* s_vals = (float*)alloca(LEN_1D * sizeof(float));

        // Vectorized computation of s_vals[i] = b[i] + c[i]
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);
        for (; i < limit; i += 8) {
            s_vals[i+0] = b[i+0] + c[i+0];
            s_vals[i+1] = b[i+1] + c[i+1];
            s_vals[i+2] = b[i+2] + c[i+2];
            s_vals[i+3] = b[i+3] + c[i+3];
            s_vals[i+4] = b[i+4] + c[i+4];
            s_vals[i+5] = b[i+5] + c[i+5];
            s_vals[i+6] = b[i+6] + c[i+6];
            s_vals[i+7] = b[i+7] + c[i+7];
        }
        for (; i < LEN_1D; i++) {
            s_vals[i] = b[i] + c[i];
        }

        // First element uses s (which is 0.0)
        a[0] = s * e[0];
        b[0] = a[0] + d[0];

        // Vectorized loop for i = 1 to LEN_1D-1
        // a[i] = s_vals[i-1] * e[i]
        // b[i] = a[i] + d[i]
        i = 1;
        int limit2 = 1 + ((LEN_1D - 1) - ((LEN_1D - 1) % 8));
        for (; i < limit2; i += 8) {
            float ai0 = s_vals[i-1+0] * e[i+0];
            float ai1 = s_vals[i-1+1] * e[i+1];
            float ai2 = s_vals[i-1+2] * e[i+2];
            float ai3 = s_vals[i-1+3] * e[i+3];
            float ai4 = s_vals[i-1+4] * e[i+4];
            float ai5 = s_vals[i-1+5] * e[i+5];
            float ai6 = s_vals[i-1+6] * e[i+6];
            float ai7 = s_vals[i-1+7] * e[i+7];
            a[i+0] = ai0; b[i+0] = ai0 + d[i+0];
            a[i+1] = ai1; b[i+1] = ai1 + d[i+1];
            a[i+2] = ai2; b[i+2] = ai2 + d[i+2];
            a[i+3] = ai3; b[i+3] = ai3 + d[i+3];
            a[i+4] = ai4; b[i+4] = ai4 + d[i+4];
            a[i+5] = ai5; b[i+5] = ai5 + d[i+5];
            a[i+6] = ai6; b[i+6] = ai6 + d[i+6];
            a[i+7] = ai7; b[i+7] = ai7 + d[i+7];
        }
        for (; i < LEN_1D; i++) {
            float ai = s_vals[i-1] * e[i];
            a[i] = ai;
            b[i] = ai + d[i];
        }
    }
}
