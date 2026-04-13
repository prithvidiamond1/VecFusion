#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool vectorized_AllPointsEq(const int pts[], int count) {
    if (count <= 1) return true;

    int val = pts[0];

    typedef int v8si __attribute__((vector_size(32)));

    int i = 1;
    int result = 1;

    if (count - 1 >= 8) {
        v8si splat = {val, val, val, val, val, val, val, val};
        v8si ones  = {1, 1, 1, 1, 1, 1, 1, 1};
        v8si acc   = {1, 1, 1, 1, 1, 1, 1, 1};

        for (; i + 7 < count; i += 8) {
            v8si chunk;
            __builtin_memcpy(&chunk, &pts[i], sizeof(v8si));
            v8si cmp = (chunk == splat);
            v8si mask = cmp & ones;
            acc = acc & mask;
        }

        result = acc[0] & acc[1] & acc[2] & acc[3] &
                 acc[4] & acc[5] & acc[6] & acc[7];
    }

    for (; i < count; ++i) {
        result &= (pts[0] == pts[i]);
    }

    return (bool)result;
}
