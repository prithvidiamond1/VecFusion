a[i] = (!cond) * a_update + cond * a[i];
c[i+1] = cond * c_update + (!cond) * c[i+1];
