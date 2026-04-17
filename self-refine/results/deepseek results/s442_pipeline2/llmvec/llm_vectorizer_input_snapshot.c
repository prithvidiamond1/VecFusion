for (int nl = 0; nl < nl_iterations; nl++) {
    for (int i = 0; i < LEN_1D; i++) {
        float temp1 = b[i] * b[i];
        float temp2 = c[i] * c[i];
        float temp3 = d[i] * d[i];
        float temp4 = e[i] * e[i];
        
        switch (indx[i]) {
            case 1: a[i] += temp1; break;
            case 2: a[i] += temp2; break;
            case 3: a[i] += temp3; break;
            case 4: a[i] += temp4; break;
        }
    }
}