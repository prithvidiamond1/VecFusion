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

    float scaleIfScalable    = normalCase ? scale : 0.0f;
    float scaleIfNonScalable = normalCase ? 1.0f  : scale;

    int   curSrc = srcStart;
    float curDst = dstStart;
    bool  curIsScalable = isScalable;

    int i = 0;

    // Process 4 at a time (4 flips = net no change to isScalable)
    for (; i + 3 < divCount; i += 4) {
        float s0 = curIsScalable ? scaleIfScalable : scaleIfNonScalable;
        float s1 = curIsScalable ? scaleIfNonScalable : scaleIfScalable;
        float s2 = curIsScalable ? scaleIfScalable : scaleIfNonScalable;
        float s3 = curIsScalable ? scaleIfNonScalable : scaleIfScalable;

        int d0 = divs[i];
        int d1 = divs[i + 1];
        int d2 = divs[i + 2];
        int d3 = divs[i + 3];

        int delta0 = d0 - curSrc;
        int delta1 = d1 - d0;
        int delta2 = d2 - d1;
        int delta3 = d3 - d2;

        float dstDelta0 = s0 * (float)delta0;
        float dstDelta1 = s1 * (float)delta1;
        float dstDelta2 = s2 * (float)delta2;
        float dstDelta3 = s3 * (float)delta3;

        src[i + 1] = d0;
        src[i + 2] = d1;
        src[i + 3] = d2;
        src[i + 4] = d3;

        dst[i + 1] = curDst + dstDelta0;
        dst[i + 2] = curDst + dstDelta0 + dstDelta1;
        dst[i + 3] = curDst + dstDelta0 + dstDelta1 + dstDelta2;
        dst[i + 4] = curDst + dstDelta0 + dstDelta1 + dstDelta2 + dstDelta3;

        curSrc = d3;
        curDst = dst[i + 4];
        // curIsScalable unchanged after 4 flips
    }

    // Process 2 at a time (2 flips = net no change to isScalable)
    for (; i + 1 < divCount; i += 2) {
        float s0 = curIsScalable ? scaleIfScalable : scaleIfNonScalable;
        float s1 = curIsScalable ? scaleIfNonScalable : scaleIfScalable;

        int d0 = divs[i];
        int d1 = divs[i + 1];

        int delta0 = d0 - curSrc;
        int delta1 = d1 - d0;

        float dstDelta0 = s0 * (float)delta0;
        float dstDelta1 = s1 * (float)delta1;

        src[i + 1] = d0;
        src[i + 2] = d1;

        dst[i + 1] = curDst + dstDelta0;
        dst[i + 2] = curDst + dstDelta0 + dstDelta1;

        curSrc = d1;
        curDst = dst[i + 2];
        // curIsScalable unchanged after 2 flips
    }

    // Scalar tail for remaining element
    for (; i < divCount; i++) {
        int d = divs[i];
        int srcDelta = d - curSrc;
        float dstDelta;
        if (normalCase) {
            dstDelta = curIsScalable ? scale * (float)srcDelta : (float)srcDelta;
        } else {
            dstDelta = curIsScalable ? 0.0f : scale * (float)srcDelta;
        }
        src[i + 1] = d;
        dst[i + 1] = curDst + dstDelta;
        curSrc = d;
        curDst = dst[i + 1];
        curIsScalable = !curIsScalable;
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
