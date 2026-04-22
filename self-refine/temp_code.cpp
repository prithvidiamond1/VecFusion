
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
    
    // Pre-compute all src positions first (independent)
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    
    // Compute all srcDelta values in a separate vectorizable loop
    int* srcDelta = (int*)__builtin_alloca(divCount * sizeof(int));
    for (int i = 0; i < divCount; i++) {
        srcDelta[i] = src[i + 1] - src[i];
    }
    
    // Compute scalable flags without loop-carried dependency
    int* scalable_flag = (int*)__builtin_alloca(divCount * sizeof(int));
    int start_flag = isScalable ? 1 : 0;
    for (int i = 0; i < divCount; i++) {
        scalable_flag[i] = start_flag ^ (i & 1);
    }
    
    // Compute dstDelta_temp using arithmetic masking (no branching)
    float* dstDelta_temp = (float*)__builtin_alloca(divCount * sizeof(float));
    if (normal_case) {
        // dstDelta = isScalable ? scale * srcDelta : srcDelta
        for (int i = 0; i < divCount; i++) {
            float fixed_part = (float)srcDelta[i] * (1.0f - scalable_flag[i]);
            float scalable_part = scale * (float)srcDelta[i] * scalable_flag[i];
            dstDelta_temp[i] = fixed_part + scalable_part;
        }
    } else {
        // dstDelta = isScalable ? 0.0f : scale * srcDelta
        for (int i = 0; i < divCount; i++) {
            dstDelta_temp[i] = scale * (float)srcDelta[i] * (1.0f - scalable_flag[i]);
        }
    }
    
    // Compute dst positions with prefix sum
    // This dependency remains but is isolated in a simple loop
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta_temp[i];
    }
    
    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
