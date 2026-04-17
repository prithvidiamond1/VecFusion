#include <stdint.h>

void vectorized_s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = 1;
        for (int i = 0; i < 256; i++) {
            // j goes from 1 to 255, k starts at current k value
            // bb[j][i] = bb[j-1][i] + flat_2d_array[k-1] * cc[j][i]
            // k increments each j iteration, then one more after inner loop
            // So flat_2d_array indices are k-1, k, k+1, ... for j=1,2,3,...
            // This is a recurrence: bb[j][i] depends on bb[j-1][i]
            // We cannot vectorize across j due to the recurrence.
            // However, we can unroll manually.
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j-1][i] + flat_2d_array[k-1] * cc[j][i];
                ++k;
            }
            ++k;
        }
    }
}
