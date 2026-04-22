
void s293_opt(int iterations,int LEN_1D, float* a) {
    // Loop splitting: separate i=0 case from others
    // i=0 case is self-assignment, can be removed entirely
    // Broadcast a[0] to all other elements
    float broadcast_val;
    
    for (int nl = 0; nl < 4*iterations; nl++) {
        // Read a[0] once per outer iteration
        broadcast_val = a[0];
        
        // Vector-friendly loop: no loop-carried dependency
        // All writes are independent after reading a[0]
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = broadcast_val;
        }
        // i=0 case is implicit: a[0] = broadcast_val (no-op)
    }
}
