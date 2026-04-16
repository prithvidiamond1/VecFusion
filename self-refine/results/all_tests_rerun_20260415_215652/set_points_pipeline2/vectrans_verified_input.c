void set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed, int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd, bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normalCase = (srcFixed <= dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    // Precompute scale factors for scalable and non-scalable patches
    float scaleWhenScalable    = normalCase ? scale : 0.0f;
    float scaleWhenNonScalable = normalCase ? 1.0f  : scale;

    // Fill src array (no dependency, can be vectorized)
    src[0] = srcStart;
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    src[divCount + 1] = srcEnd;

    // Precompute srcDelta array
    int* srcDelta = (int*)__builtin_alloca((divCount + 1) * sizeof(int));
    for (int i = 0; i < divCount; i++) {
        srcDelta[i] = src[i + 1] - src[i];
    }

    // Precompute dstDelta array using alternating isScalable (no dependency, vectorizable)
    float* dstDelta = (float*)__builtin_alloca((divCount + 1) * sizeof(float));
    int curScalable = (int)isScalable;
    for (int i = 0; i < divCount; i++) {
        float s = curScalable ? scaleWhenScalable : scaleWhenNonScalable;
        dstDelta[i] = s * (float)srcDelta[i];
        curScalable = !curScalable;
    }

    // Accumulate dst (sequential due to dependency)
    dst[0] = dstStart;
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta[i];
    }
    dst[divCount + 1] = dstEnd;
}