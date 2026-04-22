for (int j = 1; j < 256; j++) {
    for (int i = 1; i < 256; i++) {
        aa[j][i] = (aa[j][i-1] + aa[j-1][i]) / 1.9;
    }
}
