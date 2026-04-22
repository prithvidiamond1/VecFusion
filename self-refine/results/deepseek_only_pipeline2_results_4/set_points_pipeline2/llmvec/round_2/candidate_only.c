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

    // Pre-compute src[i+1] values into a temporary array to avoid corrupting input src
    int* src_temp = (int*)__builtin_alloca((divCount + 2) * sizeof(int));
    src_temp[0] = src[0];
    
    int i = 0;
    for (; i + 3 < divCount; i += 4) {
        v4i div_vec = *(v4i*)&divs[i];
        *(v4i*)&src_temp[i + 1] = div_vec;
    }
    for (; i < divCount; i++) {
        src_temp[i + 1] = divs[i];
    }
    src_temp[divCount + 1] = srcEnd;

    // Compute dstDelta values directly from src_temp without modifying src
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
