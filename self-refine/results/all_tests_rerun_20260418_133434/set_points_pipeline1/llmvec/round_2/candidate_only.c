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

    src[0] = srcStart;
    dst[0] = dstStart;

    float scaleIfScalable    = normalCase ? scale : 0.0f;
    float scaleIfNonScalable = normalCase ? 1.0f  : scale;

    int i = 0;

    // Unrolled by 4
    for (; i <= divCount - 4; i += 4) {
        bool is0 = isScalable;
        bool is1 = !is0;
        bool is2 = is0;
        bool is3 = !is0;

        float sc0 = is0 ? scaleIfScalable : scaleIfNonScalable;
        float sc1 = is1 ? scaleIfScalable : scaleIfNonScalable;
        float sc2 = is2 ? scaleIfScalable : scaleIfNonScalable;
        float sc3 = is3 ? scaleIfScalable : scaleIfNonScalable;

        src[i+1] = divs[i+0];
        src[i+2] = divs[i+1];
        src[i+3] = divs[i+2];
        src[i+4] = divs[i+3];

        float dstDelta0 = sc0 * (float)(src[i+1] - src[i+0]);
        float dstDelta1 = sc1 * (float)(src[i+2] - src[i+1]);
        float dstDelta2 = sc2 * (float)(src[i+3] - src[i+2]);
        float dstDelta3 = sc3 * (float)(src[i+4] - src[i+3]);

        dst[i+1] = dst[i+0] + dstDelta0;
        dst[i+2] = dst[i+1] + dstDelta1;
        dst[i+3] = dst[i+2] + dstDelta2;
        dst[i+4] = dst[i+3] + dstDelta3;

        // 4 flips: isScalable unchanged
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
