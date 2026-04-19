void s141_opt(int iterations, float * flat_2d_array, float bb[256][256])
{
    for (int nl = 0; nl < 200*(iterations/256); nl++) {
        // Loop interchange: j outer, i inner for contiguous access
        for (int j = 0; j < 256; j++) {
            int base = j * (j + 1) / 2;  // k for i=0
            // Vectorizable inner loop: contiguous accesses in both arrays
            for (int i = 0; i <= j; i++) {
                flat_2d_array[base + i] += bb[j][i];
            }
        }
    }
}
