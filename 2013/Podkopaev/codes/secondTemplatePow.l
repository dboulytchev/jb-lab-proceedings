{ { read(k); read(n) };
  r := 1;
  while 
    (k > 0)
  do 
    { if ((k % 2) != 0) then r := (r * n)
                  else skip;
      n := (n * n);
      k := (k / 2) };
  write(r) }