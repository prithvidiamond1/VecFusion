#include <stddef.h>

void vectorized_s481(int iterations, int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int exit_flag = 0;
        int exit_index = LEN_1D;
        
        // Vectorized search for first negative value
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        const int VF = 4;
        
        // Process vector chunks
        for (; i + VF <= LEN_1D; i += VF) {
            v4f dv = *(v4f*)&d[i];
            v4f mask = dv < (v4f){0.0f, 0.0f, 0.0f, 0.0f};
            
            // Check if any lane is negative
            int any_negative = 0;
            for (int lane = 0; lane < VF; lane++) {
                if (mask[lane]) {
                    any_negative = 1;
                    break;
                }
            }
            
            if (any_negative) {
                // Find first negative within this vector
                for (int lane = 0; lane < VF; lane++) {
                    if (d[i + lane] < 0.0f) {
                        exit_flag = 1;
                        exit_index = i + lane;
                        break;
                    }
                }
                break;
            }
        }
        
        // Scalar tail for search
        if (!exit_flag) {
            for (; i < LEN_1D; i++) {
                if (d[i] < 0.0f) {
                    exit_flag = 1;
                    exit_index = i;
                    break;
                }
            }
        }
        
        // Vectorized computation up to exit_index
        int j = 0;
        for (; j + VF <= exit_index; j += VF) {
            v4f av = *(v4f*)&a[j];
            v4f bv = *(v4f*)&b[j];
            v4f cv = *(v4f*)&c[j];
            av += bv * cv;
            *(v4f*)&a[j] = av;
        }
        
        // Scalar tail for computation
        for (; j < exit_index; j++) {
            a[j] += b[j] * c[j];
        }
        
        if (exit_flag) {
            return;
        }
    }
}
