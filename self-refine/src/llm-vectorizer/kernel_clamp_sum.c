void clamp_sum(const int *a, const int *b, int *out, int n) {
    for (int i = 0; i < n; ++i) {
        int sum = a[i] + b[i];
        if (sum > 500) {
            sum = 500;
        } else if (sum < -500) {
            sum = -500;
        }
        out[i] = sum;
    }
}
