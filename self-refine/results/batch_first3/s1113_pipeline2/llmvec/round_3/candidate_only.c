#include <stdint.h>

typedef float v4sf __attribute__((vector_size(16)));

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float amid = a[mid];

        // Phase 1: i = 0 to mid-1
        {
            v4sf vamid = {amid, amid, amid, amid};
            int i = 0;
            int limit = mid - (mid % 4);
            for (; i < limit; i += 4) {
                v4sf vb = *((v4sf*)(b + i));
                v4sf res = vamid + vb;
                *((v4sf*)(a + i)) = res;
            }
            for (; i < mid; i++) {
                a[i] = amid + b[i];
            }
        }

        // Mid-point update
        a[mid] = amid + b[mid];
        float tmp = a[mid];

        // Phase 2: i = mid+1 to LEN_1D-1
        {
            v4sf vtmp = {tmp, tmp, tmp, tmp};
            int start = mid + 1;
            int len = LEN_1D - start;
            int i = start;
            int limit = start + (len - (len % 4));
            for (; i < limit; i += 4) {
                v4sf vb = *((v4sf*)(b + i));
                v4sf res = vtmp + vb;
                *((v4sf*)(a + i)) = res;
            }
            for (; i < LEN_1D; i++) {
                a[i] = tmp + b[i];
            }
        }
    }
}
