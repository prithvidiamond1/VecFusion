bool AllPointsEq_opt(const int pts[], int count) {
    int result = 1;
    for (int i = 1; i < count; ++i) {
        result &= (pts[0] == pts[i]);
    }
    return (bool)result;
}