#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                       int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                       bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    if (srcFixed <= dstLen) {
        // This is the "normal" case, where we scale the "scalable" patches and leave
        // the other patches fixed.
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        // In this case, we eliminate the "scalable" patches and scale the "fixed" patches.
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
        int srcDelta = src[i + 1] - src[i];
        float dstDelta;
        if (srcFixed <= dstLen) {
            dstDelta = isScalable ? scale * srcDelta : srcDelta;
        } else {
            dstDelta = isScalable ? 0.0f : scale * srcDelta;
        }
        dst[i + 1] = dst[i] + dstDelta;

        // Alternate between "scalable" and "fixed" patches.
        isScalable = !isScalable;
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}

        #include <stdbool.h>

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                           int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                           bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    int normalCase = (srcFixed <= dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;

    // Precompute scale factors for scalable and non-scalable patches
    // normalCase: scalable -> scale * srcDelta, non-scalable -> 1.0f * srcDelta
    // !normalCase: scalable -> 0.0f * srcDelta, non-scalable -> scale * srcDelta
    float scaleIfScalable    = normalCase ? scale : 0.0f;
    float scaleIfNonScalable = normalCase ? 1.0f  : scale;

    // Unroll by 4 for vector-friendly processing
    int i = 0;
    // We track isScalable as an integer (0 or 1) for easier manipulation
    int isScalableInt = isScalable ? 1 : 0;

    // For unrolled loop, we need to know the pattern of isScalable for 4 consecutive iterations
    // isScalable alternates each iteration
    for (; i + 3 < divCount; i += 4) {
        // Determine scalable flags for i, i+1, i+2, i+3
        int s0 = isScalableInt;
        int s1 = s0 ^ 1;
        int s2 = s0;
        int s3 = s0 ^ 1;

        // Load divs
        int d0 = divs[i + 0];
        int d1 = divs[i + 1];
        int d2 = divs[i + 2];
        int d3 = divs[i + 3];

        src[i + 1] = d0;
        src[i + 2] = d1;
        src[i + 3] = d2;
        src[i + 4] = d3;

        // Compute srcDeltas
        int srcDelta0 = src[i + 1] - src[i + 0];
        int srcDelta1 = src[i + 2] - src[i + 1];
        int srcDelta2 = src[i + 3] - src[i + 2];
        int srcDelta3 = src[i + 4] - src[i + 3];

        // Compute dstDeltas
        float sc0 = s0 ? scaleIfScalable : scaleIfNonScalable;
        float sc1 = s1 ? scaleIfScalable : scaleIfNonScalable;
        float sc2 = s2 ? scaleIfScalable : scaleIfNonScalable;
        float sc3 = s3 ? scaleIfScalable : scaleIfNonScalable;

        float dstDelta0 = sc0 * (float)srcDelta0;
        float dstDelta1 = sc1 * (float)srcDelta1;
        float dstDelta2 = sc2 * (float)srcDelta2;
        float dstDelta3 = sc3 * (float)srcDelta3;

        dst[i + 1] = dst[i + 0] + dstDelta0;
        dst[i + 2] = dst[i + 1] + dstDelta1;
        dst[i + 3] = dst[i + 2] + dstDelta2;
        dst[i + 4] = dst[i + 3] + dstDelta3;

        // After 4 iterations, isScalable is back to original (flipped 4 times)
        // isScalableInt unchanged
    }

    // Scalar tail
    for (; i < divCount; i++) {
        src[i + 1] = divs[i];
        int srcDelta = src[i + 1] - src[i];
        float sc = isScalableInt ? scaleIfScalable : scaleIfNonScalable;
        float dstDelta = sc * (float)srcDelta;
        dst[i + 1] = dst[i] + dstDelta;
        isScalableInt ^= 1;
    }

    src[divCount + 1] = srcEnd;
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
