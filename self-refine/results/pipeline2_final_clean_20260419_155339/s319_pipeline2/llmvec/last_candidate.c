float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    float sum = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        float sum_inner = 0.0f;
        int i = 0;

        // Vectorized main loop
        for (; i + 3 < LEN_1D; i += 4) {
            float c0 = c[i];
            float c1 = c[i + 1];
            float c2 = c[i + 2];
            float c3 = c[i + 3];

            float d0 = d[i];
            float d1 = d[i + 1];
            float d2 = d[i + 2];
            float d3 = d[i + 3];

            float e0 = e[i];
            float e1 = e[i + 1];
            float e2 = e[i + 2];
            float e3 = e[i + 3];

            float ai0 = c0 + d0;
            float ai1 = c1 + d1;
            float ai2 = c2 + d2;
            float ai3 = c3 + d3;

            float bi0 = c0 + e0;
            float bi1 = c1 + e1;
            float bi2 = c2 + e2;
            float bi3 = c3 + e3;

            a[i] = ai0;
            a[i + 1] = ai1;
            a[i + 2] = ai2;
            a[i + 3] = ai3;

            b[i] = bi0;
            b[i + 1] = bi1;
            b[i + 2] = bi2;
            b[i + 3] = bi3;

            // Match scalar reduction order exactly
            sum_inner += ai0 + bi0;
            sum_inner += ai1 + bi1;
            sum_inner += ai2 + bi2;
            sum_inner += ai3 + bi3;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float ai = c[i] + d[i];
            float bi = c[i] + e[i];
            a[i] = ai;
            b[i] = bi;
            sum_inner += ai + bi;
        }

        sum += sum_inner;
    }

    return sum;
}
