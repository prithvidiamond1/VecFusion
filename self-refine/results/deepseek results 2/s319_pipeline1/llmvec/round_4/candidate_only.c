sum = 0.;
for (int i = 0; i < LEN_1D; i++) {
    a[i] = c[i] + d[i];
    sum += a[i];
    b[i] = c[i] + e[i];
    sum += b[i];
}
