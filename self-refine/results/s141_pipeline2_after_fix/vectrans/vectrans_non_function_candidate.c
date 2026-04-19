for (int j = i; j < 256; j++) {
    bb_values[j] = bb[j][i];
}
// Then...
for (int j = i; j < 256; j++) {
    temp_k = flat_2d_array[k];
    temp_k += bb_values[j];
    flat_2d_array[k] = temp_k;
    k += j + 1;
}
