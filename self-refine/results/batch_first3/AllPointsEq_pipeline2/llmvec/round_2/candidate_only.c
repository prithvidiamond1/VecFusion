#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Scalar reference implementation matching 4-argument harness signature
bool AllPointsEq(const int* a, const int* b, bool* out, int n) {
    (void)b;
    if (n <= 0) { if (out) *out = true; return true; }
    int first = a[0];
    int result = 1;
    for (int i = 1; i < n; ++i) {
        result &= (first == a[i]);
    }
    bool r = (bool)result;
    if (out) *out = r;
    return r;
}

bool vectorized_AllPointsEq(const int* a, const int* b, bool* out, int n) {
    (void)b;
    if (n <= 0) { if (out) *out = true; return true; }

    int first = a[0];

    typedef int v8si __attribute__((vector_size(32)));

    int i = 1;
    int result = 1;

    if ((n - 1) >= 8) {
        v8si vfirst = {first, first, first, first, first, first, first, first};
        v8si vresult = {1, 1, 1, 1, 1, 1, 1, 1};

        for (; i + 7 < n; i += 8) {
            v8si vdata;
            memcpy(&vdata, &a[i], sizeof(v8si));
            v8si vcmp = (vdata == vfirst);
            vresult &= vcmp;
        }

        int tmp[8];
        memcpy(tmp, &vresult, sizeof(tmp));
        for (int j = 0; j < 8; j++) {
            result &= (tmp[j] != 0);
        }
    }

    for (; i < n; i++) {
        result &= (first == a[i]);
    }

    bool r = (bool)result;
    if (out) *out = r;
    return r;
}
