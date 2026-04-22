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

        #include <stdbool.h>

typedef float v4f __attribute__((ext_vector_type(4)));
typedef int v4i __attribute__((ext_vector_type(4)));

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
                           int srcScalable, int srcStart, int srcEnd, float dstStart, float dstEnd,
                           bool isScalable) {
    float dstLen = dstEnd - dstStart;
    float scale;
    if (srcFixed <= dstLen) {
        scale = (dstLen - ((float) srcFixed)) / ((float) srcScalable);
    } else {
        scale = dstLen / ((float) srcFixed);
    }

    dst[0] = dstStart;

    // Compute cumulative source positions into temporary array
    int* src_temp = (int*)__builtin_alloca((divCount + 2) * sizeof(int));
    src_temp[0] = srcStart;

    // Vectorized prefix sum for src_temp
    int i = 0;
    v4i accum_vec = {srcStart, 0, 0, 0};

    for (; i + 3 < divCount; i += 4) {
        v4i div_vec = *(v4i*)&divs[i];

        // Horizontal prefix sum within the vector
        div_vec.y += div_vec.x;
        div_vec.z += div_vec.y;
        div_vec.w += div_vec.z;

        // Add accumulated value from previous iteration
        div_vec += (v4i){accum_vec.w, accum_vec.w, accum_vec.w, accum_vec.w};

        // Store results
        *(v4i*)&src_temp[i + 1] = div_vec;

        // Update accumulator for next iteration
        accum_vec = (v4i){div_vec.w, div_vec.w, div_vec.w, div_vec.w};
    }

    // Scalar cleanup for remaining elements
    int accum = (i == 0) ? srcStart : src_temp[i];
    for (; i < divCount; i++) {
        accum += divs[i];
        src_temp[i + 1] = accum;
    }

    src_temp[divCount + 1] = srcEnd;

    // Compute dstDelta values
    float scaleFactor = (srcFixed <= dstLen) ? scale : 0.0f;
    float fixedFactor = (srcFixed <= dstLen) ? 1.0f : scale;
    int startFlag = isScalable ? 1 : 0;

    float* dstDelta = (float*)__builtin_alloca(divCount * sizeof(float));

    i = 0;
    for (; i + 3 < divCount; i += 4) {
        v4i src_vec0 = *(v4i*)&src_temp[i];
        v4i src_vec1 = *(v4i*)&src_temp[i + 1];
        v4i srcDelta_vec = src_vec1 - src_vec0;

        v4i flag_vec = {startFlag ^ (i & 1), startFlag ^ ((i + 1) & 1),
                        startFlag ^ ((i + 2) & 1), startFlag ^ ((i + 3) & 1)};

        v4f scaleFactor_vec = {scaleFactor, scaleFactor, scaleFactor, scaleFactor};
        v4f fixedFactor_vec = {fixedFactor, fixedFactor, fixedFactor, fixedFactor};

        v4f term1 = scaleFactor_vec * (v4f)srcDelta_vec * (v4f)flag_vec;
        v4f term2 = fixedFactor_vec * (v4f)srcDelta_vec * (v4f)(1 - flag_vec);

        v4f dstDelta_vec = term1 + term2;
        *(v4f*)&dstDelta[i] = dstDelta_vec;
    }

    for (; i < divCount; i++) {
        int srcDelta = src_temp[i + 1] - src_temp[i];
        int flag = startFlag ^ (i & 1);
        dstDelta[i] = scaleFactor * srcDelta * flag + fixedFactor * srcDelta * (1 - flag);
    }

    // Prefix sum for dst
    float accum0 = dstStart;
    i = 0;

    for (; i + 3 < divCount; i += 4) {
        v4f delta_vec = *(v4f*)&dstDelta[i];

        float d0 = delta_vec.x;
        float d1 = delta_vec.y;
        float d2 = delta_vec.z;
        float d3 = delta_vec.w;

        accum0 += d0;
        float accum1 = accum0 + d1;
        float accum2 = accum1 + d2;
        float accum3 = accum2 + d3;

        dst[i + 1] = accum0;
        dst[i + 2] = accum1;
        dst[i + 3] = accum2;
        dst[i + 4] = accum3;

        accum0 = accum3;
    }

    for (; i < divCount; i++) {
        accum0 += dstDelta[i];
        dst[i + 1] = accum0;
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
