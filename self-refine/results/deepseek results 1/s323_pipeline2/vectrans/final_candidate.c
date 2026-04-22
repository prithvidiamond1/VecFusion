for (int i = 1; i < LEN_1D; i++) {
    a[i] = b[i-1] + c[i] * d[i];
    b[i] = a[i] + c[i] * e[i];
}
