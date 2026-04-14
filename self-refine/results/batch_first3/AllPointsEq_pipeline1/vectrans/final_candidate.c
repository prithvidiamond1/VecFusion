bool AllPointsEq_opt(const int pts[], int count) {
    int val = pts[0];
    int eq = 1;
    for (int i = 1; i < count; ++i) {
        eq &= (val == pts[i]);
    }
    return (bool)eq;
}
