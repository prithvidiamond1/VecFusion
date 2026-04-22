
bool AllPointsEq_opt(const int pts[], int count) {
    if (count <= 1) return true;
    
    const int first = pts[0];
    int diff_or = 0;
    
    // Process all elements without early exit
    for (int i = 1; i < count; ++i) {
        diff_or |= (first ^ pts[i]);
    }
    
    return diff_or == 0;
}
