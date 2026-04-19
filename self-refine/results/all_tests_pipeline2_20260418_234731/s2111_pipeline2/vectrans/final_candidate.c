void s2111_opt(int iterations, float aa[256][256])
{
    int outer_loops = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loops; nl++) {
        // Process first row (j=1) - only horizontal dependency
        for (int i = 1; i < 256; i++) {
            aa[1][i] = (aa[1][i-1] + aa[0][i]) / 1.9f;
        }
        
        // Process remaining rows (j=2 to 255)
        for (int j = 2; j < 256; j++) {
            // Process first element of current row
            float first_val = (aa[j][0] + aa[j-1][1]) / 1.9f;
            
            // Process remaining elements with pipelined approach
            float prev_horizontal = first_val;
            for (int i = 2; i < 256; i++) {
                float new_val = (prev_horizontal + aa[j-1][i]) / 1.9f;
                aa[j][i-1] = prev_horizontal;  // Store previous result
                prev_horizontal = new_val;
            }
            aa[j][255] = prev_horizontal;  // Store last element
        }
    }
}
