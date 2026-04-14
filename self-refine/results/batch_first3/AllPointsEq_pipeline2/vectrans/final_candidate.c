bool AllPointsEq_opt(const int pts[], int count) {
    int first = pts[0];
    int result = 1;
    for (int i = 1; i < count; ++i) {
        result &= (first == pts[i]);
    }
    return (bool)result;
}
