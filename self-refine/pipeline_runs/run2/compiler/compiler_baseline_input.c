void saxpy_like(const int *a, const int *b, int *out, int n) {
    for (int i = 0; i < n; ++i) {
        out[i] = a[i] + 2 * b[i];
    }
}
