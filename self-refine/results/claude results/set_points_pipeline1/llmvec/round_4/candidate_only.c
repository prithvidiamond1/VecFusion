#include <stdbool.h>

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                           int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                           bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normalCase = (srcFixed <= (int)dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    float scaleIfScalable    = normalCase ? scale : 0.0f;
    float scaleIfNonScalable = normalCase ? 1.0f  : scale;

    src[0] = srcStart;
    dst[0] = dstStart;

    int curSrc = srcStart;
    float curDst = dstStart;
    int isScalableInt = isScalable ? 1 : 0;

    int i = 0;

    // Unrolled by 4
    for (; i + 3 < divCount; i += 4) {
        int s0 = isScalableInt;
        int s1 = s0 ^ 1;
        int s2 = s0;
        int s3 = s0 ^ 1;

        int d0 = divs[i + 0];
        int d1 = divs[i + 1];
        int d2 = divs[i + 2];
        int d3 = divs[i + 3];

        int srcDelta0 = d0 - curSrc;
        int srcDelta1 = d1 - d0;
        int srcDelta2 = d2 - d1;
        int srcDelta3 = d3 - d2;

        float sc0 = s0 ? scaleIfScalable : scaleIfNonScalable;
        float sc1 = s1 ? scaleIfScalable : scaleIfNonScalable;
        float sc2 = s2 ? scaleIfScalable : scaleIfNonScalable;
        float sc3 = s3 ? scaleIfScalable : scaleIfNonScalable;

        float dstDelta0 = sc0 * (float)srcDelta0;
        float dstDelta1 = sc1 * (float)srcDelta1;
        float dstDelta2 = sc2 * (float)srcDelta2;
        float dstDelta3 = sc3 * (float)srcDelta3;

        src[i + 1] = d0;
        src[i + 2] = d1;
        src[i + 3] = d2;
        src[i + 4] = d3;

        dst[i + 1] = curDst + dstDelta0;
        dst[i + 2] = dst[i + 1] + dstDelta1;
        dst[i + 3] = dst[i + 2] + dstDelta2;
        dst[i + 4] = dst[i + 3] + dstDelta3;

        curSrc = d3;
        curDst = dst[i + 4];
        // isScalableInt unchanged after 4 flips (correct)
    }

    // Scalar tail
    for (; i < divCount; i++) {
        int d = divs[i];
        int srcDelta = d - curSrc;
        float sc = isScalableInt ? scaleIfScalable : scaleIfNonScalable;
        float dstDelta = sc * (float)srcDelta;

        src[i + 1] = d;
        dst[i + 1] = curDst + dstDelta;

        curSrc = d;
        curDst = dst[i + 1];
        isScalableInt ^= 1;
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
