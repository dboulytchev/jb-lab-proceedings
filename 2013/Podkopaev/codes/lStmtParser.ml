ostap (
      ident    : !(Lexer.ident);
      key[name]: @(name ^ "\\b" : name);
      parse[expr][stmt]: 
        x:ident ":=" e:expr                                {`Assign (x, e)}
      | key["skip"]                                        {`Skip}
      | key["read" ] "(" x:ident ")"                       {`Read x}
      | key["write"] "(" e:expr ")"                        {`Write e}
      
      | key["if"] c:expr key["then"]
          x:parse[expr][stmt] key["else"]
          y:parse[expr][stmt]                              {`If (c, x, y)}
      
      | key["while"] c:expr key["do"] s:parse[expr][stmt]  {`While (c, s)}

      | -"{" -s:parse[expr][stmt] -";" seqs[s][expr][stmt] -"}"
      | stmt[parse expr stmt];                                                          
      seqs[acc][expr][stmt]: s:parse[expr][stmt]
        t:(-";" seqs[`Seq (acc, s)][expr][stmt])? {
          match t with Some t -> t | None -> `Seq (acc, s)
        }
    )