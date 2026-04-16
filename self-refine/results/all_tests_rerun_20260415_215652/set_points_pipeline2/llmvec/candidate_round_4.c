#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed, int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd, bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normalCase = (srcFixed <= dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    // Precompute scale factors for scalable and non-scalable patches
    float scaleWhenScalable    = normalCase ? scale : 0.0f;
    float scaleWhenNonScalable = normalCase ? 1.0f  : scale;

    // Fill src array (no dependency, can be vectorized)
    src[0] = srcStart;
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }
    src[divCount + 1] = srcEnd;

    // Precompute srcDelta array
    int* srcDelta = (int*)__builtin_alloca((divCount + 1) * sizeof(int));
    for (int i = 0; i < divCount; i++) {
        srcDelta[i] = src[i + 1] - src[i];
    }

    // Precompute dstDelta array using alternating isScalable (no dependency, vectorizable)
    float* dstDelta = (float*)__builtin_alloca((divCount + 1) * sizeof(float));
    int curScalable = (int)isScalable;
    for (int i = 0; i < divCount; i++) {
        float s = curScalable ? scaleWhenScalable : scaleWhenNonScalable;
        dstDelta[i] = s * (float)srcDelta[i];
        curScalable = !curScalable;
    }

    // Accumulate dst (sequential due to dependency)
    dst[0] = dstStart;
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta[i];
    }
    dst[divCount + 1] = dstEnd;
}

        #include <stdbool.h>
#include <alloca.h>

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed, int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd, bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normalCase = (srcFixed <= dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    float scaleWhenScalable    = normalCase ? scale : 0.0f;
    float scaleWhenNonScalable = normalCase ? 1.0f  : scale;

    // Fill src array (write back to caller's src)
    src[0] = srcStart;
    {
        int i = 0;
        for (; i + 3 < divCount; i += 4) {
            src[i + 1] = divs[i];
            src[i + 2] = divs[i + 1];
            src[i + 3] = divs[i + 2];
            src[i + 4] = divs[i + 3];
        }
        for (; i < divCount; i++) {
            src[i + 1] = divs[i];
        }
    }
    src[divCount + 1] = srcEnd;

    int totalSegs = divCount + 1;

    // Precompute srcDelta array: totalSegs elements
    int* srcDelta = (int*)alloca(totalSegs * sizeof(int));
    {
        int i = 0;
        for (; i + 3 < totalSegs; i += 4) {
            srcDelta[i]     = src[i + 1] - src[i];
            srcDelta[i + 1] = src[i + 2] - src[i + 1];
            srcDelta[i + 2] = src[i + 3] - src[i + 2];
            srcDelta[i + 3] = src[i + 4] - src[i + 3];
        }
        for (; i < totalSegs; i++) {
            srcDelta[i] = src[i + 1] - src[i];
        }
    }

    // Precompute dstDelta array: totalSegs elements
    // Even segments: scalable when isScalable==true, non-scalable when isScalable==false
    // Odd segments: non-scalable when isScalable==true, scalable when isScalable==false
    float* dstDelta = (float*)alloca(totalSegs * sizeof(float));
    {
        float scaleEven = isScalable ? scaleWhenScalable    : scaleWhenNonScalable;
        float scaleOdd  = isScalable ? scaleWhenNonScalable : scaleWhenScalable;

        int i = 0;
        for (; i + 3 < totalSegs; i += 4) {
            dstDelta[i]     = scaleEven * (float)srcDelta[i];
            dstDelta[i + 1] = scaleOdd  * (float)srcDelta[i + 1];
            dstDelta[i + 2] = scaleEven * (float)srcDelta[i + 2];
            dstDelta[i + 3] = scaleOdd  * (float)srcDelta[i + 3];
        }
        for (; i < totalSegs; i++) {
            float s = ((i % 2) == 0) ? scaleEven : scaleOdd;
            dstDelta[i] = s * (float)srcDelta[i];
        }
    }

    // Accumulate dst (sequential due to dependency)
    dst[0] = dstStart;
    for (int i = 0; i < totalSegs; i++) {
        dst[i + 1] = dst[i] + dstDelta[i];
    }
    dst[divCount + 1] = dstEnd;
}

        static uint32_t next_u32(uint32_t *state) {
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }

        static void fill_i32(int *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }
        }

        static void fill_f32(float *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
            }
        }

        static void fill_f64(double *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
            }
        }

        int main(void) {
            const int n = 128;
            uint32_t seed = 7u;
            float dst_scalar[128]; float dst_vector[128]; int src_scalar[128]; int src_vector[128]; int divs[128]; int divCount = n; int srcFixed = 7; int srcScalable = 7; int srcStart = 7; int srcEnd = 7; float dstStart = 7; float dstEnd = 7; bool isScalable = 7;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(dst_scalar, n, &seed); memcpy(dst_vector, dst_scalar, sizeof(dst_scalar)); fill_i32(src_scalar, n, &seed); memcpy(src_vector, src_scalar, sizeof(src_scalar)); fill_i32(divs, n, &seed);
                set_points(dst_scalar, src_scalar, divs, divCount, srcFixed, srcScalable, srcStart, srcEnd, dstStart, dstEnd, isScalable); vectorized_set_points(dst_vector, src_vector, divs, divCount, srcFixed, srcScalable, srcStart, srcEnd, dstStart, dstEnd, isScalable);
                for (int i = 0; i < n; ++i) {
    if (fabsf((dst_scalar[i]) - (dst_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter dst on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if ((src_scalar[i]) != (src_vector[i])) {
        fprintf(stderr, "Mismatch in parameter src on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
