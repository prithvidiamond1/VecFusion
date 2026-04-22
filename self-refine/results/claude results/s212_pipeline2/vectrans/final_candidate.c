void s212_opt(int n, float *a, float *b, float *c, float *d) {
    // Save original a[1..n-1] values before they get updated
    // b[i] uses a[i+1] which is the ORIGINAL (not yet multiplied by c[i+1]) value
    // We need to store original a values first, then do the updates
    
    // Use a temp copy of a[1..n-1] (the values used by b[i])
    float *a_orig = (float *)__builtin_alloca((n - 1) * sizeof(float));
    memcpy(a_orig, a + 1, (n - 1) * sizeof(float));

    // Now update a[i] *= c[i] - vectorizable
    for (int i = 0; i < n - 1; i++) {
        a[i] *= c[i];
    }

    // Now update b[i] using original a[i+1] values - vectorizable
    for (int i = 0; i < n - 1; i++) {
        b[i] += a_orig[i] * d[i];
    }
}
