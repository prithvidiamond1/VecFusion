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
    int normalCase = (srcFixed <= (int)dstLen);
    if (normalCase) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    src[0] = srcStart;
    dst[0] = dstStart;

    // Precompute scale factors:
    // scaleA: scale to apply when isScalable matches initial isScalable (even iterations)
    // scaleB: scale to apply when isScalable is flipped (odd iterations)
    float scaleA, scaleB;
    if (normalCase) {
        scaleA = isScalable ? scale : 1.0f;
        scaleB = isScalable ? 1.0f  : scale;
    } else {
        scaleA = isScalable ? 0.0f  : scale;
        scaleB = isScalable ? scale : 0.0f;
    }

    int i = 0;

    // Unroll by 4: after 4 iterations isScalable returns to original state
    for (; i <= divCount - 4; i += 4) {
        src[i+1] = divs[i+0];
        src[i+2] = divs[i+1];
        src[i+3] = divs[i+2];
        src[i+4] = divs[i+3];

        float d0 = scaleA * (float)(src[i+1] - src[i+0]);
        float d1 = scaleB * (float)(src[i+2] - src[i+1]);
        float d2 = scaleA * (float)(src[i+3] - src[i+2]);
        float d3 = scaleB * (float)(src[i+4] - src[i+3]);

        dst[i+1] = dst[i+0] + d0;
        dst[i+2] = dst[i+1] + d1;
        dst[i+3] = dst[i+2] + d2;
        dst[i+4] = dst[i+3] + d3;
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            float dst_scalar[128]; float dst_vector[128]; int src_scalar[128]; int src_vector[128]; int divs[128]; int divCount = arr_len; int srcFixed = 7; int srcScalable = 7; int srcStart = 7; int srcEnd = 7; float dstStart = 7; float dstEnd = 7; bool isScalable = 7;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(dst_scalar, arr_len, &seed); memcpy(dst_vector, dst_scalar, sizeof(dst_scalar)); fill_i32(src_scalar, arr_len, &seed); memcpy(src_vector, src_scalar, sizeof(src_scalar)); fill_i32(divs, arr_len, &seed);
                set_points(dst_scalar, src_scalar, divs, divCount, srcFixed, srcScalable, srcStart, srcEnd, dstStart, dstEnd, isScalable); vectorized_set_points(dst_vector, src_vector, divs, divCount, srcFixed, srcScalable, srcStart, srcEnd, dstStart, dstEnd, isScalable);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((dst_scalar[i]) - (dst_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter dst on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if ((src_scalar[i]) != (src_vector[i])) {
        fprintf(stderr, "Mismatch in parameter src on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
