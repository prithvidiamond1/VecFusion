float prev_bb = bb[0][i];
for (int j = 1; j < 256; j++) {
    float new_val = prev_bb + flat_2d_array[k-1] * cc[j][i];
    prev_bb = bb[j][i];
    bb[j][i] = new_val;
    ++k;
}
