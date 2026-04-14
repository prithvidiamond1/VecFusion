#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef int v8si __attribute__((vector_size(32)));

void vectorized_AllPointsEq(int count, const int pts[], int *out_vector, int n) {
    for (int call = 0; call < n; ++call) {
        if (count <= 1) {
            out_vector[call] = 1;
            continue;
        }

        int val = pts[0];
        int result = 1;

        v8si vval = {val, val, val, val, val, val, val, val};

        int i = 1;
        int remaining = count - 1;
        int simd_count = (remaining / 8) * 8;
        int simd_end = 1 + simd_count;
        if (simd_end > count) simd_end = count;

        for (; i < simd_end; i += 8) {
            v8si chunk;
            memcpy(&chunk, &pts[i], sizeof(v8si));
            v8si cmp = (chunk != vval);
            int r = cmp[0] | cmp[1] | cmp[2] | cmp[3] |
                    cmp[4] | cmp[5] | cmp[6] | cmp[7];
            if (r) {
                result = 0;
                break;
            }
        }

        if (result) {
            for (; i < count; ++i) {
                if (pts[i] != val) {
                    result = 0;
                    break;
                }
            }
        }

        out_vector[call] = result;
    }
}
