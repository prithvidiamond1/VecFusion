#include <stddef.h>

void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d) {
    float s_sum = s1 + s2;
    int loop_bound = iterations / 5;
    
    for (int nl = 0; nl < loop_bound; nl++) {
        float prev = a[0];
        
        // Compute independent parts with explicit bounds
        int i = 1;
        int main_loop_bound = LEN_1D - 1;
        
        // Vector-friendly loop for independent computations
        for (; i <= main_loop_bound; i++) {
            // Independent computation - no dependencies
            a[i] = s_sum + b[i] + c[i] + d[i];
        }
        
        // Perform recurrence accumulation with explicit bounds
        prev = a[0];
        for (i = 1; i < LEN_1D; i++) {
            prev = prev + a[i];
            a[i] = prev;
        }
    }
}
