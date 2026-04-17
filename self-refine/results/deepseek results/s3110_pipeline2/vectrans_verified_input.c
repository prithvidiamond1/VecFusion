for (int j = 1; j < 256; j++) {
    float current = aa[i][j];
    if (current > row_max) {
        row_max = current;
        row_yindex = j;
    }
}