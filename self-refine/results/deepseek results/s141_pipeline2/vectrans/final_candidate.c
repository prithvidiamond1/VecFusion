int k_start = (i+1) * i / 2 + i;
int k = k_start;
for (int j = i; j < 256; j++) {
    int current_k = k;
    k += j + 1;
    flat_2d_array[current_k] += bb[j][i];
}
