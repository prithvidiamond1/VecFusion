void max_pair(const int *a, const int *b, int *out, int n) {
    for (int i = 0; i < n; ++i) {
        out[i] = (a[i] > b[i]) ? a[i] : b[i];
    }
}
