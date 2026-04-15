bool AllPointsEq_opt(const int pts[], int count) {
    int val = pts[0];
    int all_eq = 1;
    for (int i = 1; i < count; ++i) {
        all_eq &= (val == pts[i]);
    }
    return (bool)all_eq;
}
