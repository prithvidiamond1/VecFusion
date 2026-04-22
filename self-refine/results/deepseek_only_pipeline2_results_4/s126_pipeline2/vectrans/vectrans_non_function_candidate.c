for (int j = 1; j < 256; j++) {
    temp_store[j] = ...;
    ++k;
}
// k now = 256
for (int j = 1; j < 256; j++) {
    bb[j][i] = ...; // no k increment here
}
++k; // k now = 257
