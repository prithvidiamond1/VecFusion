bool AllPointsEq(const int pts[], int count) {
    int ref = pts[0];
    int eq = 1;
    for (int i = 1; i < count; ++i) {
        eq &= (pts[i] == ref);
    }
    return (bool)eq;
}