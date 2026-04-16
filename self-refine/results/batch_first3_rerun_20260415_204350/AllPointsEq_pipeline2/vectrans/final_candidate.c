bool AllPointsEq_opt(const int pts[], int count) {
    int mismatch = 0;
    for (int i = 1; i < count; ++i) {
        mismatch |= (pts[0] != pts[i]);
    }
    return !mismatch;
}
