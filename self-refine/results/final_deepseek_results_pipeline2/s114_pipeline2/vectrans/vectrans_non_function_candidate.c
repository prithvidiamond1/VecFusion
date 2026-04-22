float temp_row[256];
for (int j = 0; j < i; j++) {
    temp_row[j] = aa[j][i];  // read upper triangle
}
for (int j = 0; j < i; j++) {
    aa[i][j] = temp_row[j] + bb[i][j]; // write lower triangle
}
