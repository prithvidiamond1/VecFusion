void set_points_opt(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                    int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                    bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normal_case = srcFixed <= dstLen;
    
    if (normal_case) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;
    
    // Pre-compute all src[i+1] values first
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    
    // Compute all srcDelta values into a temporary array
    int* srcDelta_arr = (int*)alloca(divCount * sizeof(int));
    for (int i = 0; i < divCount; i++) {
        srcDelta_arr[i] = src[i + 1] - src[i];
    }
    
    // Compute all dstDelta values into a temporary array
    float* dstDelta_arr = (float*)alloca(divCount * sizeof(float));
    float scale_fixed = normal_case ? 1.0f : scale;
    float scale_scalable = normal_case ? scale : 0.0f;
    
    // Process all segments with alternating scaling rules
    // Use two separate loops for even and odd indices to remove branching
    if (isScalable) {
        // First segment is scalable
        for (int i = 0; i < divCount; i += 2) {
            dstDelta_arr[i] = scale_scalable * srcDelta_arr[i];
        }
        for (int i = 1; i < divCount; i += 2) {
            dstDelta_arr[i] = scale_fixed * srcDelta_arr[i];
        }
    } else {
        // First segment is fixed
        for (int i = 0; i < divCount; i += 2) {
            dstDelta_arr[i] = scale_fixed * srcDelta_arr[i];
        }
        for (int i = 1; i < divCount; i += 2) {
            dstDelta_arr[i] = scale_scalable * srcDelta_arr[i];
        }
    }
    
    // Compute cumulative sum of dstDelta values
    float cumulative = dstStart;
    for (int i = 0; i < divCount; i++) {
        cumulative += dstDelta_arr[i];
        dst[i + 1] = cumulative;
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
