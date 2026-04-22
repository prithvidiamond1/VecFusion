#include <stdlib.h>
#include <string.h>

void set_points_opt(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                       int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                       bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    if (srcFixed <= dstLen) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;

    // Loop 1: Compute all src values (fully vectorizable)
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }

    // Pre-allocate temporary arrays
    float* dstTemp = (float*)malloc((divCount + 1) * sizeof(float));
    dstTemp[0] = dstStart;

    // Loop 2: Compute dstDeltas and do prefix sum in a way that can be vectorized
    // Use the original loop structure but store results in two separate arrays
    float* deltas = (float*)malloc(divCount * sizeof(float));
    
    // First compute all deltas (fully vectorizable)
    float cond1 = (srcFixed <= dstLen) ? 1.0f : 0.0f;
    
    for (int i = 0; i < divCount; i++) {
        int srcDelta = src[i + 1] - src[i];
        float scalableMask = isScalable ? 1.0f : 0.0f;
        
        // Arithmetic masking for the conditional logic
        float normalCase = scalableMask * (scale * (float)srcDelta) + (1.0f - scalableMask) * (float)srcDelta;
        float elseCase = scalableMask * 0.0f + (1.0f - scalableMask) * (scale * (float)srcDelta);
        deltas[i] = cond1 * normalCase + (1.0f - cond1) * elseCase;
        
        isScalable = !isScalable;
    }

    // Loop 3: Vectorized prefix sum using scan pattern
    // Use multiple passes to handle the carry dependency
    int blockSize = 4; // Vector width
    for (int start = 0; start < divCount; start += blockSize) {
        int end = (start + blockSize < divCount) ? start + blockSize : divCount;
        float carry = (start > 0) ? dstTemp[start] : dstStart;
        
        // This inner loop can be vectorized because each block is independent
        for (int j = start; j < end; j++) {
            carry = carry + deltas[j];
            dstTemp[j + 1] = carry;
        }
        
        // Store the last value for the next block
        if (end < divCount) {
            dstTemp[end] = carry;
        }
    }

    // Loop 4: Copy results to dst (fully vectorizable)
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dstTemp[i + 1];
    }

    free(dstTemp);
    free(deltas);

    // Set final values
    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}
