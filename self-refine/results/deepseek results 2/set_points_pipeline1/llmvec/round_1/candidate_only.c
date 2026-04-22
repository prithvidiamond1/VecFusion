#include <stdbool.h>

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                           int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                           bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normalCase = (srcFixed <= dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;

    // Precompute scale factors for scalable and non-scalable patches
    // In normal case: scalable -> scale * srcDelta, non-scalable -> srcDelta
    // In non-normal case: scalable -> 0.0f, non-scalable -> scale * srcDelta
    float scaleIfScalable    = normalCase ? scale  : 0.0f;
    float scaleIfNonScalable = normalCase ? 1.0f   : scale;

    // Unroll by 4 for vector-friendly processing
    int i = 0;
    // We need to track isScalable state across iterations
    // Process in chunks of 4, but we need to handle alternating isScalable carefully
    // For each group of 4 consecutive iterations starting at i, the isScalable pattern is:
    // i+0: isScalable (current), i+1: !isScalable, i+2: isScalable, i+3: !isScalable
    // We precompute scale factors for each position in the group

    for (; i <= divCount - 4; i += 4) {
        // Load src values
        int s0 = src[i];
        int d0 = divs[i];
        int d1 = divs[i+1];
        int d2 = divs[i+2];
        int d3 = divs[i+3];

        src[i+1] = d0;
        src[i+2] = d1;
        src[i+3] = d2;
        src[i+4] = d3;

        int srcDelta0 = d0 - s0;
        int srcDelta1 = d1 - d0;
        int srcDelta2 = d2 - d1;
        int srcDelta3 = d3 - d2;

        // Determine scale for each based on isScalable alternation
        bool is0 = isScalable;
        bool is1 = !is0;
        bool is2 = !is1;
        bool is3 = !is2;

        float sc0 = is0 ? scaleIfScalable : scaleIfNonScalable;
        float sc1 = is1 ? scaleIfScalable : scaleIfNonScalable;
        float sc2 = is2 ? scaleIfScalable : scaleIfNonScalable;
        float sc3 = is3 ? scaleIfScalable : scaleIfNonScalable;

        float dstDelta0 = sc0 * (float)srcDelta0;
        float dstDelta1 = sc1 * (float)srcDelta1;
        float dstDelta2 = sc2 * (float)srcDelta2;
        float dstDelta3 = sc3 * (float)srcDelta3;

        float base = dst[i];
        dst[i+1] = base + dstDelta0;
        dst[i+2] = base + dstDelta0 + dstDelta1;
        dst[i+3] = base + dstDelta0 + dstDelta1 + dstDelta2;
        dst[i+4] = base + dstDelta0 + dstDelta1 + dstDelta2 + dstDelta3;

        // After 4 iterations, isScalable returns to original value (flipped 4 times)
        // isScalable unchanged
    }

    // Scalar tail
    for (; i < divCount; i++) {
        src[i + 1] = divs[i];
        int srcDelta = src[i + 1] - src[i];
        float dstDelta;
        if (normalCase) {
            dstDelta = isScalable ? scale * (float)srcDelta : (float)srcDelta;
        } else {
            dstDelta = isScalable ? 0.0f : scale * (float)srcDelta;
        }
        dst[i + 1] = dst[i] + dstDelta;
        isScalable = !isScalable;
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
