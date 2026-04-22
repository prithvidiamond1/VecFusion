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
    float normal_scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    float shrink_scale = dstLen / ((float) srcFixed);

    int use_normal = srcFixed <= dstLen;
    scale = use_normal ? normal_scale : shrink_scale;

    src[0] = srcStart;
    dst[0] = dstStart;

    // Pre-calculate all src[i+1] and srcDelta values
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }

    // Calculate all dstDelta values without dependencies
    float dstDelta[divCount];
    int current_scalable = isScalable;

    for (int i = 0; i < divCount; i++) {
        int srcDelta = src[i + 1] - src[i];

        if (use_normal) {
            dstDelta[i] = current_scalable ? scale * srcDelta : srcDelta;
        } else {
            dstDelta[i] = current_scalable ? 0.0f : scale * srcDelta;
        }

        current_scalable = !current_scalable;
    }

    // Sequential accumulation for dst[i+1]
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta[i];
    }

    src[divCount + 1] = srcEnd;
    dst[divCount + 1] = dstEnd;
}

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

    dst[0] = dstStart;

    // Write src[1..divCount] from divs
    for (int i = 0; i < divCount; i++) {
        src[i + 1] = divs[i];
    }

    // Calculate all dstDelta values
    float dstDelta[divCount > 0 ? divCount : 1];

    int scalable_even = (int)isScalable;
    int scalable_odd  = (int)(!isScalable);

    if (use_normal) {
        int i = 0;
        for (; i <= divCount - 4; i += 4) {
            int prev0 = (i == 0) ? srcStart : divs[i - 1];
            int prev1 = divs[i];
            int prev2 = divs[i + 1];
            int prev3 = divs[i + 2];

            int srcDelta0 = divs[i]     - prev0;
            int srcDelta1 = divs[i + 1] - prev1;
            int srcDelta2 = divs[i + 2] - prev2;
            int srcDelta3 = divs[i + 3] - prev3;

            int s0 = (i % 2 == 0) ? scalable_even : scalable_odd;
            int s1 = !s0;
            int s2 = s0;
            int s3 = s1;

            dstDelta[i]     = s0 ? scale * (float)srcDelta0 : (float)srcDelta0;
            dstDelta[i + 1] = s1 ? scale * (float)srcDelta1 : (float)srcDelta1;
            dstDelta[i + 2] = s2 ? scale * (float)srcDelta2 : (float)srcDelta2;
            dstDelta[i + 3] = s3 ? scale * (float)srcDelta3 : (float)srcDelta3;
        }
        int current_scalable = (i % 2 == 0) ? scalable_even : scalable_odd;
        for (; i < divCount; i++) {
            int prev = (i == 0) ? srcStart : divs[i - 1];
            int srcDelta = divs[i] - prev;
            dstDelta[i] = current_scalable ? scale * (float)srcDelta : (float)srcDelta;
            current_scalable = !current_scalable;
        }
    } else {
        int i = 0;
        for (; i <= divCount - 4; i += 4) {
            int prev0 = (i == 0) ? srcStart : divs[i - 1];
            int prev1 = divs[i];
            int prev2 = divs[i + 1];
            int prev3 = divs[i + 2];

            int srcDelta0 = divs[i]     - prev0;
            int srcDelta1 = divs[i + 1] - prev1;
            int srcDelta2 = divs[i + 2] - prev2;
            int srcDelta3 = divs[i + 3] - prev3;

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
            int prev = (i == 0) ? srcStart : divs[i - 1];
            int srcDelta = divs[i] - prev;
            dstDelta[i] = current_scalable ? 0.0f : scale * (float)srcDelta;
            current_scalable = !current_scalable;
        }
    }

    // Sequential accumulation for dst[i+1]
    for (int i = 0; i < divCount; i++) {
        dst[i + 1] = dst[i] + dstDelta[i];
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
