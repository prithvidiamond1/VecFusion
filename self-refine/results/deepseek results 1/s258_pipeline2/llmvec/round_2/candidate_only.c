for (i = 0; i < n; i++) {
    if (b[i] > 0.0f) s = d[i] * d[i];
    a[i] = s * c[i] + d[i];
    e[i] = (s + 1.0f) * aa[0][i];
}
