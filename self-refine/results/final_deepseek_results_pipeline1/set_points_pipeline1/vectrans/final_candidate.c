void set_points_opt(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                       int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                       bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    if (srcFixed <= dstLen) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;

    // Compute all src values first (can be vectorized)
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    src[divCount + 1] = srcEnd;

    // Compute scale factors using arithmetic to avoid branching
    float scaleScalable, scaleFixed;
    if (srcFixed <= dstLen) {
        scaleScalable = scale;
        scaleFixed = 1.0f;
    } else {
        scaleScalable = 0.0f;
        scaleFixed = scale;
    }

    // Precompute dstDelta values (no loop-carried dependency in this separated loop)
    float dstDelta[divCount + 1];
    for (int i = 0; i < divCount; i++) {
        int srcDelta = src[i + 1] - src[i];
        float isScalableVal = (isScalable ? 1.0f : 0.0f);
        dstDelta[i] = isScalableVal * scaleScalable * srcDelta + 
                      (1.0f - isScalableVal) * scaleFixed * srcDelta;
        isScalable = !isScalable;
    }

    // Compute dst values using prefix scan (can be vectorized with reduction)
    float runningSum = dst[0];
    for (int i = 0; i < divCount; i++) {
        runningSum += dstDelta[i];
        dst[i + 1] = runningSum;
    }

    dst[divCount + 1] = dstEnd;
}
