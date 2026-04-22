for (int i = 1; i < LEN_1D-1; i++) {
    a[i] = b[i - 1] + c[i] * d[i];
    b[i] = b[i + 1] - e[i] * d[i];
}
