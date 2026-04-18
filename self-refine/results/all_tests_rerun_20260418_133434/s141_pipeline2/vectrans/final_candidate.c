k = base_k;
for (int j = i; j < 256; j++) {
    k_offsets[i][j] = k;
    k += j + 1;
}
