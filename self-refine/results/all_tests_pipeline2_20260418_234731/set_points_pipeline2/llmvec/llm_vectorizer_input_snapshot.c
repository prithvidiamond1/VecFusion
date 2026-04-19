void set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
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

    // Pre-compute all src[i+1] values (vectorizable)
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }

    // Compute all srcDelta values (vectorizable)
    // Use src array directly to avoid extra allocation
    src[1] -= srcStart;  // src[1] now stores srcDelta[0]
    for (int i = 1; i < divCount; i++) {
        src[i + 1] -= src[i];  // src[i+1] now stores srcDelta[i]
    }

    // Restore src values for later use
    src[1] += srcStart;
    for (int i = 1; i < divCount; i++) {
        src[i + 1] += src[i];
    }

    // Compute dstDelta values with branchless logic (vectorizable)
    float scaleFactor = (srcFixed <= dstLen) ? scale : 0.0f;
    float fixedFactor = (srcFixed <= dstLen) ? 1.0f : scale;
    int startFlag = isScalable ? 1 : 0;
    
    // Temporary array for dstDelta
    float* dstDelta = (float*)__builtin_alloca(divCount * sizeof(float));
    
    for (int i = 0; i < divCount; i++) {
        int srcDelta = src[i + 1] - src[i];
        int flag = startFlag ^ (i & 1);
        dstDelta[i] = scaleFactor * srcDelta * flag + fixedFactor * srcDelta * (1 - flag);
    }

    // Prefix sum for dst using multiple accumulators for better vectorization
    float accum0 = dstStart;
    float accum1 = 0.0f, accum2 = 0.0f, accum3 = 0.0f;
    int i;
    
    // Process in chunks of 4
    for (i = 0; i + 3 < divCount; i += 4) {
        // Load 4 dstDelta values
        float d0 = dstDelta[i];
        float d1 = dstDelta[i + 1];
        float d2 = dstDelta[i + 2];
        float d3 = dstDelta[i + 3];
        
        // Parallel accumulation
        accum0 += d0;
        accum1 = accum0 + d1;
        accum2 = accum1 + d2;
        accum3 = accum2 + d3;
        
        // Store results
        dst[i + 1] = accum0;
        dst[i + 2] = accum1;
        dst[i + 3] = accum2;
        dst[i + 4] = accum3;
        
        // Update main accumulator
        accum0 = accum3;
    }
    
    // Handle remaining elements
    for (; i < divCount; i++) {
        accum0 += dstDelta[i];
        dst[i + 1] = accum0;
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}