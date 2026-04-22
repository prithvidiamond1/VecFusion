void s442_opt(int iterations,int LEN_1D, float* a, float *b,float* c, float *d, float* e, int* indx)
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
