let rec parse primary s =  
  let l = List.map (fun (s, t) -> ostap(- $(s)), fun x y -> `Binop (t, s, x, y)) in
  let ior  x y = abs x + abs y in
  let iand x y = abs (x * y) in
  let b f = fun x y -> if f x y then 1 else 0 in
  Ostap.Util.expr (fun x -> x) [|
    `Lefta, l ["||", ior];
    `Lefta, l ["&&", iand];
    `Nona , l ["==", b(=); "!=", b(<>); "<=", b(<=); "<", b(<); ">=", b(>=); ">", b(>)];
    `Lefta, l ["+" , ( +  ); "-" , (-)];
    `Lefta, l ["*" , ( *  ); "/" , (/ ); "%" , (mod)];
  |]
  (primary (parse primary))
  s