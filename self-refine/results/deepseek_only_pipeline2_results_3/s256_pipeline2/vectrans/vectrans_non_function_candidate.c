for (int j = 1; j < 256; j++) {
    a[j] = (float)1.0 - a[j - 1];
    aa[j][i] = a[j] + bb[j][i] * d[j];
}
