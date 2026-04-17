float s317(int iterations, int LEN_1D) {
    float q = (float)1.;
    for (int i = 0; i < LEN_1D/2; i++) {
        q *= (float).99;
    }
    return q;
}