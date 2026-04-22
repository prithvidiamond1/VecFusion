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

        #include <stdbool.h>

typedef float v4f __attribute__((ext_vector_type(4)));
typedef int v4i __attribute__((ext_vector_type(4)));

void vectorized_set_points(float* dst, int* src, const int* divs, int divCount, int srcFixed,
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

    // Vectorized pre-computation of src positions
    int i = 0;
    for (; i + 3 < divCount; i += 4) {
        v4i div_vec = *(v4i*)&divs[i];
        *(v4i*)&src[i + 1] = div_vec;
    }
    for (; i < divCount; i++) {
        src[i + 1] = divs[i];
    }

    // Vectorized computation of srcDelta values
    int* srcDelta = (int*)__builtin_alloca(divCount * sizeof(int));
    i = 0;
    for (; i + 3 < divCount; i += 4) {
        v4i src_vec1 = *(v4i*)&src[i];
        v4i src_vec2 = *(v4i*)&src[i + 1];
        v4i delta_vec = src_vec2 - src_vec1;
        *(v4i*)&srcDelta[i] = delta_vec;
    }
    for (; i < divCount; i++) {
        srcDelta[i] = src[i + 1] - src[i];
    }

    // Compute scaling factors
    float scale_fixed = normal_case ? 1.0f : scale;
    float scale_scalable = normal_case ? scale : 0.0f;

    // Generate alternating pattern
    int start_mask = isScalable ? 1 : 0;
    float* dstDelta = (float*)__builtin_alloca(divCount * sizeof(float));

    // Vectorized computation of dstDelta values
    i = 0;
    v4f scale_fixed_vec = (v4f){scale_fixed, scale_fixed, scale_fixed, scale_fixed};
    v4f scale_scalable_vec = (v4f){scale_scalable, scale_scalable, scale_scalable, scale_scalable};

    for (; i + 3 < divCount; i += 4) {
        // Load srcDelta values
        v4i srcDelta_vec = *(v4i*)&srcDelta[i];
        v4f srcDelta_fvec = (v4f){(float)srcDelta_vec[0], (float)srcDelta_vec[1], 
                                  (float)srcDelta_vec[2], (float)srcDelta_vec[3]};

        // Compute alternating pattern masks
        int mask0 = ((i + 0) % 2 == 0) ? start_mask : (1 - start_mask);
        int mask1 = ((i + 1) % 2 == 0) ? start_mask : (1 - start_mask);
        int mask2 = ((i + 2) % 2 == 0) ? start_mask : (1 - start_mask);
        int mask3 = ((i + 3) % 2 == 0) ? start_mask : (1 - start_mask);

        // Select appropriate scale for each element
        v4f effective_scale = (v4f){
            mask0 ? scale_scalable : scale_fixed,
            mask1 ? scale_scalable : scale_fixed,
            mask2 ? scale_scalable : scale_fixed,
            mask3 ? scale_scalable : scale_fixed
        };

        // Compute dstDelta
        v4f dstDelta_vec = srcDelta_fvec * effective_scale;
        *(v4f*)&dstDelta[i] = dstDelta_vec;
    }

    // Scalar tail for dstDelta
    for (; i < divCount; i++) {
        int is_scalable_mask = (i % 2 == 0) ? start_mask : (1 - start_mask);
        float effective_scale = is_scalable_mask ? scale_scalable : scale_fixed;
        dstDelta[i] = effective_scale * srcDelta[i];
    }

    // Cumulative sum (serial dependency remains scalar)
    for (i = 0; i < divCount; i++) {
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
