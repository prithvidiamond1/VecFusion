#include <stdint.h>
#include <stdbool.h>
#include <alloca.h>
#include <string.h>

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normal_case = srcFixed <= dstLen;

    if (normal_case) {
        scale = (dstLen - ((float)srcFixed)) / ((float)srcScalable);
    } else {
        scale = dstLen / ((float)srcFixed);
    }

    dst[0] = dstStart;

    // Fill src[i] = divs[i] for i in [0, divCount)
    for (int i = 0; i < divCount; i++) {
        src[i] = divs[i];
    }

    // Compute srcDeltas: srcDeltas[0] = divs[0] - srcStart, srcDeltas[i] = divs[i] - divs[i-1]
    int* srcDeltas = (int*)alloca(divCount * sizeof(int));
    if (divCount > 0) {
        srcDeltas[0] = divs[0] - srcStart;
    }
    for (int i = 1; i < divCount; i++) {
        srcDeltas[i] = divs[i] - divs[i - 1];
    }

    // Compute dstDeltas using vectorized even/odd pattern
    float* dstDeltas = (float*)alloca(divCount * sizeof(float));

    if (normal_case) {
        if (isScalable) {
            // even: scale * srcDeltas[i], odd: (float)srcDeltas[i]
            int i = 0;
            for (; i + 3 < divCount; i += 4) {
                dstDeltas[i + 0] = scale * (float)srcDeltas[i + 0];
                dstDeltas[i + 1] = (float)srcDeltas[i + 1];
                dstDeltas[i + 2] = scale * (float)srcDeltas[i + 2];
                dstDeltas[i + 3] = (float)srcDeltas[i + 3];
            }
            for (; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? (scale * (float)srcDeltas[i]) : (float)srcDeltas[i];
            }
        } else {
            // even: (float)srcDeltas[i], odd: scale * srcDeltas[i]
            int i = 0;
            for (; i + 3 < divCount; i += 4) {
                dstDeltas[i + 0] = (float)srcDeltas[i + 0];
                dstDeltas[i + 1] = scale * (float)srcDeltas[i + 1];
                dstDeltas[i + 2] = (float)srcDeltas[i + 2];
                dstDeltas[i + 3] = scale * (float)srcDeltas[i + 3];
            }
            for (; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? (float)srcDeltas[i] : (scale * (float)srcDeltas[i]);
            }
        }
    } else {
        if (isScalable) {
            // even: 0.0f, odd: scale * srcDeltas[i]
            int i = 0;
            for (; i + 3 < divCount; i += 4) {
                dstDeltas[i + 0] = 0.0f;
                dstDeltas[i + 1] = scale * (float)srcDeltas[i + 1];
                dstDeltas[i + 2] = 0.0f;
                dstDeltas[i + 3] = scale * (float)srcDeltas[i + 3];
            }
            for (; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? 0.0f : (scale * (float)srcDeltas[i]);
            }
        } else {
            // even: scale * srcDeltas[i], odd: 0.0f
            int i = 0;
            for (; i + 3 < divCount; i += 4) {
                dstDeltas[i + 0] = scale * (float)srcDeltas[i + 0];
                dstDeltas[i + 1] = 0.0f;
                dstDeltas[i + 2] = scale * (float)srcDeltas[i + 2];
                dstDeltas[i + 3] = 0.0f;
            }
            for (; i < divCount; i++) {
                dstDeltas[i] = (i % 2 == 0) ? (scale * (float)srcDeltas[i]) : 0.0f;
            }
        }
    }

    // Sequential prefix sum for dst
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDeltas[i];
    }

    dst[divCount] = dstEnd;
}
