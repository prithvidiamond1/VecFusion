void set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
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
    
    // Pre-compute all src positions (loop distribution)
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    
    // Pre-compute all srcDelta values (independent computation)
    int* srcDelta = (int*)alloca(divCount * sizeof(int));
    for (int i = 0; i < divCount; i++) {
        srcDelta[i] = src[i + 1] - src[i];
    }
    
    // Compute scaling factors for both cases
    float scale_fixed = normal_case ? 1.0f : scale;
    float scale_scalable = normal_case ? scale : 0.0f;
    
    // Generate alternating pattern without serial dependency
    int start_mask = isScalable ? 1 : 0;
    float* dstDelta = (float*)alloca(divCount * sizeof(float));
    
    // Compute all dstDelta values in parallel (no branching)
    for (int i = 0; i < divCount; i++) {
        int is_scalable_mask = (i % 2 == 0) ? start_mask : (1 - start_mask);
        float effective_scale = is_scalable_mask ? scale_scalable : scale_fixed;
        dstDelta[i] = effective_scale * srcDelta[i];
    }
    
    // Perform cumulative sum (serial dependency isolated)
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta[i];
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}