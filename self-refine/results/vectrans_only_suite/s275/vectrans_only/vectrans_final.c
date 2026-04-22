
void s275_opt(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int outer_loop_count = 10 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Collect valid columns (loop splitting)
        int valid_cols[256];
        int valid_count = 0;
        for (int i = 0; i < 256; i++) {
            if (aa[0][i] > (float)0.) {
                valid_cols[valid_count++] = i;
            }
        }
        
        // Process only valid columns
        for (int col_idx = 0; col_idx < valid_count; col_idx++) {
            int i = valid_cols[col_idx];
            float current = aa[0][i];
            
            // Process column with explicit recurrence
            for (int j = 1; j < 256; j++) {
                current = current + bb[j][i] * cc[j][i];
                aa[j][i] = current;
            }
        }
    }
}
