int write_pos[256];
write_pos[0] = base_k + mask[0];
for (int j = 1; j < 256; j++) {
    write_pos[j] = write_pos[j-1] + mask[j];
}
for (int j = 0; j < 256; j++) {
    if (mask[j]) flat_2d_array[write_pos[j]-1] = aa[j][i];
}
