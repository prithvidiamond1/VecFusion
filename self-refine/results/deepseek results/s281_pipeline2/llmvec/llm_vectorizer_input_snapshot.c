for (int i = 0; i < LEN_1D; i++) {
    x = a[LEN_1D-i-1] + b[i] * c[i];
    a[i] = x - 1.0f;
    b[i] = x;
}