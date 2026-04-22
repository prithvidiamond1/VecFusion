for (int nl = 0; nl < 10*(iterations/256); nl++) {
    // compute a[1..255] once per nl iteration
    for (int j = 1; j < 256; j++) {
        a[j] = 1.0f - a[j - 1];
    }
    for (int i = 0; i < 256; i++) {
        // vectorizable
        for (int j = 1; j < 256; j++) {
            aa[j][i] = a[j] + bb[j][i] * d[j];
        }
    }
}