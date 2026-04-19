
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <string.h>

void set_points_opt(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                    int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                    bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normal_case = (srcFixed <= dstLen);
    
    if (normal_case) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;
    
    // Pre-compute all srcDelta values
    int srcDelta[divCount];
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
        srcDelta[i] = src[i + 1] - src[i];
    }
    
    // Pre-compute all dstDelta values based on pattern
    float dstDelta[divCount];
    if (normal_case) {
        // Pattern: scalable, fixed, scalable, fixed, ...
        for (int i = 0; i < divCount; i++) {
            int scalable_flag = (isScalable ^ (i & 1)) ? 1 : 0;
            dstDelta[i] = scalable_flag ? scale * srcDelta[i] : (float)srcDelta[i];
        }
    } else {
        // Pattern: scalable, fixed, scalable, fixed, ...
        for (int i = 0; i < divCount; i++) {
            int scalable_flag = (isScalable ^ (i & 1)) ? 1 : 0;
            dstDelta[i] = scalable_flag ? 0.0f : scale * srcDelta[i];
        }
    }
    
    // Compute dst values using prefix sum (can be vectorized with parallel prefix sum)
    float running_sum = dstStart;
    for (int i = 0; i < divCount; i++) {
        running_sum += dstDelta[i];
        dst[i + 1] = running_sum;
    }
    
    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
