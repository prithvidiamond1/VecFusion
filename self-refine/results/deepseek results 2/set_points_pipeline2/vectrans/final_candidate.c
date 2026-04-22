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
    
    // Pre-compute all src[i+1] values (vectorizable)
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    
    // Pre-compute all srcDeltas (vectorizable)
    int* srcDeltas = (int*)alloca(divCount * sizeof(int));
    srcDeltas[0] = src[1] - srcStart;
    for (int i = 1; i < divCount; i++) {
        srcDeltas[i] = src[i + 1] - src[i];
    }
    
    // Pre-compute dstDelta values based on scalable/fixed pattern
    float* dstDeltas = (float*)alloca(divCount * sizeof(float));
    
    if (normal_case) {
        // Compute all dstDeltas without branching in the loop
        if (isScalable) {
            // Pattern: scalable, fixed, scalable, fixed, ...
            for (int i = 0; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? (scale * srcDeltas[i]) : ((float)srcDeltas[i]);
            }
        } else {
            // Pattern: fixed, scalable, fixed, scalable, ...
            for (int i = 0; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? ((float)srcDeltas[i]) : (scale * srcDeltas[i]);
            }
        }
    } else {
        // srcFixed > dstLen case
        if (isScalable) {
            // Pattern: scalable(0), fixed(scaled), scalable(0), fixed(scaled), ...
            for (int i = 0; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? 0.0f : (scale * srcDeltas[i]);
            }
        } else {
            // Pattern: fixed(scaled), scalable(0), fixed(scaled), scalable(0), ...
            for (int i = 0; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? (scale * srcDeltas[i]) : 0.0f;
            }
        }
    }
    
    // Compute cumulative dst values (sequential dependency remains)
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDeltas[i];
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
