#include <stdbool.h>
#include <stdint.h>

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                           int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                           bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float normal_scale = (dstLen - ((float)srcFixed)) / ((float)srcScalable);
    float shrink_scale = dstLen / ((float)srcFixed);

    int use_normal = srcFixed <= dstLen;
    float scale = use_normal ? normal_scale : shrink_scale;

    src[0] = srcStart;
    dst[0] = dstStart;

    // Pre-calculate all src[i+1] values
    // Unroll by 4 for vectorization
    int i = 0;
    for (; i <= divCount - 4; i += 4) {
        src[i + 1] = divs[i];
        src[i + 2] = divs[i + 1];
        src[i + 3] = divs[i + 2];
        src[i + 4] = divs[i + 3];
    }
    for (; i < divCount; i++) {
        src[i + 1] = divs[i];
    }

    // Calculate all dstDelta values
    // Use a VLA for dstDelta
    float dstDelta[divCount > 0 ? divCount : 1];

    // Precompute even/odd scalable flags
    // even indices (0,2,4,...) have current_scalable = isScalable
    // odd indices (1,3,5,...) have current_scalable = !isScalable
    int scalable_even = (int)isScalable;
    int scalable_odd  = !isScalable;

    if (use_normal) {
        // dstDelta[i] = scalable ? scale * srcDelta : srcDelta
        i = 0;
        for (; i <= divCount - 4; i += 4) {
            int srcDelta0 = src[i + 1] - src[i];
            int srcDelta1 = src[i + 2] - src[i + 1];
            int srcDelta2 = src[i + 3] - src[i + 2];
            int srcDelta3 = src[i + 4] - src[i + 3];

            int s0 = (i % 2 == 0) ? scalable_even : scalable_odd;
            int s1 = !s0;
            int s2 = s0;
            int s3 = s1;

            dstDelta[i]     = s0 ? scale * (float)srcDelta0 : (float)srcDelta0;
            dstDelta[i + 1] = s1 ? scale * (float)srcDelta1 : (float)srcDelta1;
            dstDelta[i + 2] = s2 ? scale * (float)srcDelta2 : (float)srcDelta2;
            dstDelta[i + 3] = s3 ? scale * (float)srcDelta3 : (float)srcDelta3;
        }
        int current_scalable = scalable_even;
        // adjust current_scalable for remaining start
        // toggle for each completed iteration
        for (int j = 0; j < i; j++) current_scalable = !current_scalable;
        // reset properly
        current_scalable = (i % 2 == 0) ? scalable_even : scalable_odd;
        for (; i < divCount; i++) {
            int srcDelta = src[i + 1] - src[i];
            dstDelta[i] = current_scalable ? scale * (float)srcDelta : (float)srcDelta;
            current_scalable = !current_scalable;
        }
    } else {
        // dstDelta[i] = scalable ? 0.0f : scale * srcDelta
        i = 0;
        for (; i <= divCount - 4; i += 4) {
            int srcDelta0 = src[i + 1] - src[i];
            int srcDelta1 = src[i + 2] - src[i + 1];
            int srcDelta2 = src[i + 3] - src[i + 2];
            int srcDelta3 = src[i + 4] - src[i + 3];

            int s0 = (i % 2 == 0) ? scalable_even : scalable_odd;
            int s1 = !s0;
            int s2 = s0;
            int s3 = s1;

            dstDelta[i]     = s0 ? 0.0f : scale * (float)srcDelta0;
            dstDelta[i + 1] = s1 ? 0.0f : scale * (float)srcDelta1;
            dstDelta[i + 2] = s2 ? 0.0f : scale * (float)srcDelta2;
            dstDelta[i + 3] = s3 ? 0.0f : scale * (float)srcDelta3;
        }
        int current_scalable = (i % 2 == 0) ? scalable_even : scalable_odd;
        for (; i < divCount; i++) {
            int srcDelta = src[i + 1] - src[i];
            dstDelta[i] = current_scalable ? 0.0f : scale * (float)srcDelta;
            current_scalable = !current_scalable;
        }
    }

    // Sequential accumulation for dst[i+1]
    for (i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta[i];
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
