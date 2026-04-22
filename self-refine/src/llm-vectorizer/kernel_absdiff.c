void absdiff(const int *a, const int *b, int *out, int n) {
    for (int i = 0; i < n; ++i) {
        int diff = a[i] - b[i];
        out[i] = (diff < 0) ? -diff : diff;
    }
}
