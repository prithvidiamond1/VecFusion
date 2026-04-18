for (int j = 1; j < 256; j++) {
       float current_bb = prev_bb + products[j];
       bb[j][i] = current_bb;
       prev_bb = current_bb;
   }