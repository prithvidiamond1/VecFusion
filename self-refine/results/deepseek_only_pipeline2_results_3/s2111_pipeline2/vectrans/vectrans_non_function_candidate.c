float original_val = aa[j][i];
aa[j][i] = (prev_val + aa[j-1][i]) / 1.9f;
prev_val = original_val;
