void set_points_opt(float* dst, int* src, const int* divs, int divCount, int srcFixed, int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd, bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normalCase = (srcFixed <= (int)dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    // Fill src array (vectorizable - no loop-carried dependency)
    src[0] = srcStart;
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    src[divCount + 1] = srcEnd;

    // Precompute dstDelta for each segment (vectorizable)
    float dstDelta_arr[divCount + 1];
    for (int i = 0; i < divCount; i++) {
        int srcDelta = src[i + 1] - src[i];
        int curScalable = ((int)isScalable ^ (i & 1));
        float scaledDelta = scale * (float)srcDelta;
        float fixedDelta = (float)srcDelta;

        float dstDelta;
        if (normalCase) {
            dstDelta = curScalable ? scaledDelta : fixedDelta;
        } else {
            dstDelta = curScalable ? 0.0f : scaledDelta;
        }
        dstDelta_arr[i] = dstDelta;
    }

    // Fill dst using sequential accumulation (matches original floating-point behavior)
    dst[0] = dstStart;
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta_arr[i];
    }
    dst[divCount + 1] = dstEnd;
}
